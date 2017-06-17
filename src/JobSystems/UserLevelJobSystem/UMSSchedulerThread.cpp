#include "stdafx.h"
#include "UMSSchedulerThread.h"

#include "UMSScheduler.h"
#include "UMSWorkerThreadParameter.h"

#define SCHEDULER_THREAD_STACKSIZE 512 * 1024 //512K

UMSSchedulerThread::UMSSchedulerThread()
{
}

UMSSchedulerThread::~UMSSchedulerThread()
{
	//TODO: Shutdown
}

void UMSSchedulerThread::Initialize(UMSScheduler* scheduler)
{
	m_scheduler = scheduler;

	m_startupInfo.UmsVersion = UMS_VERSION;
	m_startupInfo.CompletionList = scheduler->m_CompletionList;
	m_startupInfo.SchedulerProc = UMSScheduler::StaticProc; //This function is called when the thread is converted to a UMS thread
	m_procParam.Scheduler = scheduler;
	m_procParam.SchedulerThread = this;
	m_startupInfo.SchedulerParam = &m_procParam;


	m_Thread = CreateThread(NULL, SCHEDULER_THREAD_STACKSIZE, StaticThreadProc, this, 0, &m_ThreadId); //Creates and starts thread (CREATE_SUSPENDED)
}

DWORD UMSSchedulerThread::ThreadProc()
{
	printf("The thread with the id '%d' is entering UMS\n", m_ThreadId);
	if (!EnterUmsSchedulingMode(&m_startupInfo)) { printf("Error %d", GetLastError()); } //Convert thread to UMS Scheduler Thread. SchedulerProc function is will be called.


	//Is null because a non-UMS thread is executing this
	PUMS_CONTEXT context = GetCurrentUmsThread(); //Can be called for scheduler/worker thread :https://msdn.microsoft.com/en-us/library/windows/desktop/dd627166(v=vs.85).aspx

	return 0;
}

void UMSSchedulerThread::SchedulingProc()
{
	PUMS_CONTEXT context = GetCurrentUmsThread(); //Can be called for scheduler/worker thread :https://msdn.microsoft.com/en-us/library/windows/desktop/dd627166(v=vs.85).aspx

	while (true)
	{
		const int EVENT_NUM = 1; HANDLE events[EVENT_NUM];
		events[0] = m_scheduler->m_CompletionEvent;
		DWORD waitValue = WaitForMultipleObjects(EVENT_NUM, events, false, INFINITE);

		if (waitValue == WAIT_OBJECT_0)
		{
			m_scheduler->QueryAndQueueReadyWorkerThreads();
		}
		

		//printf("Current ready queue size: %d\n", m_readyWorkerThreads.size());
		//DEBUGGGG:

		//TODO: need front for check
		Task* task;
		if (m_scheduler->PopTask(&task))
		{
			UMSWorkerThread* frontWorker = nullptr;
			PopReadWorker(&frontWorker);
			if (frontWorker)
			{

				frontWorker->m_CurrentTask = task;
				if (!ExecuteUmsThread(frontWorker->m_Context)) { printf("Error %d\n", GetLastError()); }

			}
		}
		else
		{
			m_scheduler->PushTask(task);
		}

		/*
		INFINITE is not wasting CPU time, because we are waiting
		If this function dispatches more events, set timeout to 0 or a certain value...
		*/
		//TODO: CHeck ready queue
		
	}
}

bool UMSSchedulerThread::PushReadyWorker(UMSWorkerThread * worker) //Intel nulstein inspiration
{
	std::lock_guard<std::mutex> guard(m_readyWorkersMutex);
	m_readyWorkerThreads.push(worker);

	return true;
}

bool UMSSchedulerThread::PopReadWorker(UMSWorkerThread ** OUT_worker) //Intel nulstein inspiration
{
	std::lock_guard<std::mutex> guard(m_readyWorkersMutex);
	if (m_readyWorkerThreads.size() > 0)
	{
		UMSWorkerThread* worker = m_readyWorkerThreads.front();
		m_readyWorkerThreads.pop();
		*OUT_worker = worker;
		return true;
	}
	else
	{
		return false;
	}
}

size_t UMSSchedulerThread::ReadyWorkerQueueSize()
{
	std::lock_guard<std::mutex> guard(m_readyWorkersMutex);
	return m_readyWorkerThreads.size();
}



DWORD WINAPI UMSSchedulerThread::StaticThreadProc(void * p)
{
	UMSSchedulerThread* thread = (UMSSchedulerThread*)p;

	return thread->ThreadProc();
}