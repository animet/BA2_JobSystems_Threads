#include "stdafx.h"
#include "FiberSchedulerThread.h"
#include "DebugStatics.h"
#include "FiberScheduler.h"
#include "Game.h"

#define FIBER_SCHEDULER_THREAD_STACKSIZE 512 * 1024 //512KB

FiberSchedulerThread::FiberSchedulerThread()
	: m_isReadyForProcessing(false)
{
}


FiberSchedulerThread::~FiberSchedulerThread()
{
}

void FiberSchedulerThread::Initialize(FiberScheduler * scheduler)
{
	m_scheduler = scheduler;

	m_Thread = CreateThread(NULL, FIBER_SCHEDULER_THREAD_STACKSIZE, StaticThreadProc, this, 0, &m_ThreadId); //Creates and starts thread. Suspends the thread: (CREATE_SUSPENDED)
}

DWORD FiberSchedulerThread::ThreadProc()
{
	printf("The thread with the id '%d' will be converted to a fiber to schedule other fibers...\n", GetCurrentThreadId());

	m_SchedulerFiberPointer = ConvertThreadToFiber(this); //scheduler thread is converted to fiber to schedule other fibers. Members of scheduler fibers are currently not needed
	if (m_SchedulerFiberPointer == NULL) { printf("Error %d", GetLastError()); }
	//Initialize fibers:
#if JOBSYSTEM_DEBUG_LOG
	char b[512];int n = 0;
	DebugStatics::AddMessage([this, &b, &n]()->char* { n = sprintf(b, "-- [%d] Fibers will be initialized for the FiberSchedulerThread (Kernel Thread) with the Id '%d'...\n", MAX_FIBERS_PER_SCHEDULER_THREAD, GetCurrentThreadId()); return b; }());
#endif
	for (int i = 0; i < MAX_FIBERS_PER_SCHEDULER_THREAD; i++)
	{
		m_fibers[i].Initialize(this);
		m_readyFibers.push(&m_fibers[i]);
	}
	m_isReadyForProcessing = true;

	std::unique_lock<std::mutex> guard(m_scheduleMutex);
	m_scheduleCondition.wait(guard);
	while (true)
	{
		//https://msdn.microsoft.com/en-us/library/windows/desktop/ms686919(v=vs.85).aspx
		while (JobCount_Unsafe() <= 0)
		{
#ifdef USE_STEPS_FOR_MEMORY_ANALYSIS
			if (Game::m_FinishedAllJobs)
			{
				Sleep(100000);
			}
#endif // USE_STEPS_FOR_MEMORY_ANALYSIS

			
			Sleep(1);
			//m_scheduleCondition.wait(guard); //The signal alone is not enough (race)
			//TODO: Steal Work VS working signal
		}

		/*
		Important: First pop ready worker, then Job!
		*/

		if (m_readyFibers.size() > 0)
		{
			FiberWorker* fiber = m_readyFibers.front();
			Job* job = nullptr;
			if (PopJob(&job))
			{
				fiber->SetJob(job);
				m_readyFibers.pop();
				m_currentExecutingFiber = fiber;
				SwitchToFiber(fiber->m_FiberPointer);
			}
		}
		
	}
	return 0;
}

bool FiberSchedulerThread::IsReadyForProcessing() const
{
	return m_isReadyForProcessing;
}

void FiberSchedulerThread::Activate()
{
	m_scheduleCondition.notify_one();
}

void FiberSchedulerThread::PushReadyFiber(FiberWorker * fiber)
{
	//Thought: Do not need to protect access because only one fiber per thread can run at the same time
	m_readyFibers.push(fiber);
}

bool FiberSchedulerThread::PushJob(Job * Job)
{
	//Spin lock may be faster: main thread is only pushing Jobs
	std::lock_guard<std::mutex> guard(m_jobsMutex);
	m_jobs.push(Job);

	return true;
}

bool FiberSchedulerThread::PopJob(Job ** OUT_Job)
{
	std::lock_guard<std::mutex> guard(m_jobsMutex);
	if (m_jobs.size() > 0)
	{
		Job* job = m_jobs.front();
		m_jobs.pop();
		*OUT_Job = job;
		return true;
	}
	else
	{
		return false;
	}
}

size_t FiberSchedulerThread::JobCount()
{
	std::lock_guard<std::mutex> guard(m_jobsMutex);
	return m_jobs.size();
}

size_t FiberSchedulerThread::JobCount_Unsafe()
{
	return m_jobs.size();
}

FiberWorker * FiberSchedulerThread::GetFiberByPointer(void * fiberPointer)
{
	for (int i = 0; i < MAX_FIBERS_PER_SCHEDULER_THREAD; ++i)
	{
		if (m_fibers[i].m_FiberPointer == fiberPointer)
			return &m_fibers[i];
	}
	return nullptr;
}

FiberWorker * FiberSchedulerThread::GetCurrentExecutingFiber()
{
	return m_currentExecutingFiber;
}

DWORD FiberSchedulerThread::StaticThreadProc(void * p)
{
	return ((FiberSchedulerThread*)p)->ThreadProc();
}
