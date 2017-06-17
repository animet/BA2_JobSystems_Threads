#include "stdafx.h"
#include "UMSScheduler.h"

#include "UMSSchedulerThreadParameter.h"
#include "ThreadingUtility.h"

//Inspiration: https://msdn.microsoft.com/en-us/library/windows/desktop/dd627187(v=vs.85).aspx
// Nulstein (Intel)

UMSScheduler::UMSScheduler()
	: m_numOfProcessors(ThreadingUtility::GetNumberOfProcessors())
{
}

UMSSchedulerThread * UMSScheduler::getMinSizedSchedulerThreadWorkerQueue()
{
	int minIdx = 0;
	for (int i = 0; i < m_numOfSchedulerThreads; i++)
	{
		if (i + 1 <= m_numOfSchedulerThreads - 1)
		{
			if (m_schedulerThreads[i].ReadyWorkerQueueSize() > m_schedulerThreads[i + 1].ReadyWorkerQueueSize())
			{
				minIdx = i + 1;
			}
		}
	}
	return &m_schedulerThreads[minIdx];
}

bool UMSScheduler::PopTask(Task ** OUT_task)
{
	std::lock_guard<std::mutex> guard(m_tasksMutex);
	if (m_tasks.size() > 0)
	{
		Task* task = m_tasks.front();
		m_tasks.pop();
		*OUT_task = task;
		return true;
	}
	else
	{
		return false;
	}
}

UMSScheduler::~UMSScheduler()
{
	//TODO: Own Shutdown function Shutdown
}

void UMSScheduler::Initialize()
{
	if (!CreateUmsCompletionList(&m_CompletionList)) { printf("Error %d", GetLastError()); }
	if (!GetUmsCompletionListEvent(m_CompletionList, &m_CompletionEvent)) { printf("Error %d", GetLastError()); }

	//Init Scheduler Threads:
	for (int i = 0; i < m_numOfProcessors - 1; i++) //N - 1
	{
		m_schedulerThreads[i].Initialize(this);
		m_schedulerThreads[i].m_ProcessorAffinity = pow(2, i);
	}
	m_numOfSchedulerThreads = m_numOfProcessors - 1;

	//Init Worker Threads:
	for (int i = 0; i < MAX_UMS_WORKER_THREADS; i++)
	{
		m_workerThreads[i].Initialize(this);
		m_workerThradsByContext[m_workerThreads[i].m_Context] = &m_workerThreads[i];
	}

	//Start Scheduler Threads:
	//for (int i = 0; i < m_numOfSchedulerThreads; i++) //N - 1
	//{
	//	if (ResumeThread(m_schedulerThreads[i].m_Thread) == (DWORD)-1) { printf("Error %d", GetLastError()); }
	//}
}

UMSScheduler * UMSScheduler::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new UMSScheduler();
	}

	return m_instance;
}

/*
(https://msdn.microsoft.com/en-us/library/windows/desktop/dd627187(v=vs.85).aspx)
The system calls this function when: 
	* EnterUmsSchedulingMode is called (schedulerParam specified by the caller)
	* UMS worker thread calls UmsThreadYield (schedulerParam specified by the caller)
	* UMS worker thrad blocks (schedulerParam is NULL)
*/
void WINAPI UMSScheduler::StaticProc(UMS_SCHEDULER_REASON reason, ULONG_PTR activationPayload, PVOID schedulerParam)
{
	PUMS_CONTEXT context = GetCurrentUmsThread(); //Can be called for scheduler/worker thread :https://msdn.microsoft.com/en-us/library/windows/desktop/dd627166(v=vs.85).aspx

	switch (reason)
	{
	case UmsSchedulerStartup: //UMS scheduler thread was created
		{
			UMSSchedulerThreadParameter* procParam = (UMSSchedulerThreadParameter*)schedulerParam;
			GetInstance()->ProcessStartup(procParam->SchedulerThread);
		}
		break;
	case UmsSchedulerThreadBlocked: //UMS worker thread blocked
		GetInstance()->ProcessBlock();
		break;
	case UmsSchedulerThreadYield:
		GetInstance()->ProcessYield(); //Missing parameter
		break;
	}
	
}

void UMSScheduler::ProcessStartup(UMSSchedulerThread * uThread)
{
	//https://msdn.microsoft.com/en-us/library/windows/desktop/dd627164(v=vs.85).aspx
	//
	//Set processor affinity: https://msdn.microsoft.com/en-us/library/windows/desktop/ms684251(v=vs.85).aspx (NUMA vs SMP????)
	SetThreadAffinityMask(uThread->m_Thread, uThread->m_ProcessorAffinity);
	
	QueryAndQueueReadyWorkerThreads();

	uThread->SchedulingProc();
}

void UMSScheduler::ProcessYield()
{
	printf("Some worker thread yielded.");
	//First query items from completions lists (https://msdn.microsoft.com/en-us/library/windows/desktop/dd627187(v=vs.85).aspx)
	QueryAndQueueReadyWorkerThreads();

}

void UMSScheduler::ProcessBlock()
{
	printf("Some worker thread blocked.");
	//First query items from completions lists (https://msdn.microsoft.com/en-us/library/windows/desktop/dd627187(v=vs.85).aspx)
	QueryAndQueueReadyWorkerThreads();
}

void UMSScheduler::QueryAndQueueReadyWorkerThreads()
{
	PUMS_CONTEXT umsThreadList;
	if (!DequeueUmsCompletionListItems(m_CompletionList, 0, &umsThreadList)) { printf("Error %d", GetLastError()); }

	PUMS_CONTEXT currContext = GetNextUmsListItem(umsThreadList); //with ref &?
	int workerThreadCounter = 0;
	while (currContext != NULL)
	{
		getMinSizedSchedulerThreadWorkerQueue()->PushReadyWorker(m_workerThradsByContext[currContext]); //add to ready queue (protect access)
		currContext = GetNextUmsListItem(umsThreadList); //with ref &?
		workerThreadCounter++;
	}
	printf("Queued %d ready worker threads\n", workerThreadCounter);
}

bool UMSScheduler::PushTask(Task * task)
{
	std::lock_guard<std::mutex> guard(m_tasksMutex);
	m_tasks.push(task);

	return true;
}

UMSScheduler* UMSScheduler::m_instance = nullptr;