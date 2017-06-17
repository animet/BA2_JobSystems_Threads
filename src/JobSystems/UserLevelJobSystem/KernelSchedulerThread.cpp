#include "stdafx.h"
#include "KernelSchedulerThread.h"
#include "DebugStatics.h"
#include "Scheduler.h"
#include "Job.h"
#include "Game.h"

#define KERNEL_THREAD_STACKSIZE 512 * 1024 //10KB

KernelSchedulerThread::KernelSchedulerThread()
	: m_isReadyForProcessing(false)
{
}


KernelSchedulerThread::~KernelSchedulerThread()
{
}

void KernelSchedulerThread::Initialize(Scheduler * scheduler)
{
	m_scheduler = scheduler;

	m_Thread = CreateThread(NULL, KERNEL_THREAD_STACKSIZE, StaticThreadProc, this, 0, &m_ThreadId); //Creates and starts thread. Suspends the thread: (CREATE_SUSPENDED)
}

DWORD KernelSchedulerThread::ThreadProc()
{
	printf("The thread with the id '%d' is ready for scheduling worker threads...\n", GetCurrentThreadId());

	//Initialize worker threads:
#if JOBSYSTEM_DEBUG_LOG
	char b[512];int n = 0;
	DebugStatics::AddMessage([this, &b, &n]()->char* { n = sprintf(b, "-- [%d] KernelWorkerThreads will be initialized for the KernelSchedulerThread with the Id '%d'...\n", MAX_WORKERTHREADS_PER_SCHEDULER_THREAD, GetCurrentThreadId()); return b; }());
#endif
	for (int i = 0; i < MAX_WORKERTHREADS_PER_SCHEDULER_THREAD; i++)
	{
		m_workers[i].Initialize(this);
		m_readyWorkers.push(&m_workers[i]);
	}
	m_isReadyForProcessing = true;

	std::unique_lock<std::mutex> guard(m_scheduleMutex);
	while (true)
	{
		while (JobCount_Unsafe() <= 0)
		{
#ifdef USE_STEPS_FOR_MEMORY_ANALYSIS
			if (Game::m_FinishedAllJobs)
			{
				Sleep(100000);
			}
#endif // USE_STEPS_FOR_MEMORY_ANALYSIS

			
			//m_scheduleCondition.wait(guard); : The reason that some jobs are not completed is the signal in combination with job count
			//TODO: Steal Work VS working signal
			Sleep(1);
		}
		
		/*
			Important: First pop ready worker, then Job!
		*/

		KernelWorkerThread* worker = nullptr;
		if (PopReadyWorker(&worker)) //Fibers do not need protected ready queue access
		{
			Job* job = nullptr;
			if (PopJob(&job))
			{
				worker->SetJob(job);
			}
			else if (job == nullptr)
			{
				PushReadyWorker(worker);
			}
		}
	}
	return 0;
}

bool KernelSchedulerThread::IsReadyForProcessing() const
{
	return m_isReadyForProcessing;
}

KernelWorkerThread * KernelSchedulerThread::FrontReadyWorker()
{

	return m_readyWorkers.front();
}

void KernelSchedulerThread::PushReadyWorker(KernelWorkerThread * worker)
{
	std::lock_guard<std::mutex> guard(m_readyWorkersMutex); //Fibers do not need this mutex...
	m_readyWorkers.push(worker);
}

Job * KernelSchedulerThread::FrontJob()
{
	std::lock_guard<std::mutex> guard(m_JobsMutex);
	return m_Jobs.front();
}

bool KernelSchedulerThread::PushJob(Job * Job)
{
	//Spinlock may be faster: main thread is only pushing Jobs
	std::lock_guard<std::mutex> guard(m_JobsMutex);
	++KernelSchedulerThread::m_pushedJobs;
	m_Jobs.push(Job);
	m_scheduleCondition.notify_one();
	
	return true;
}

bool KernelSchedulerThread::PopJob(Job ** OUT_Job)
{
	std::lock_guard<std::mutex> guard(m_JobsMutex);
	++KernelSchedulerThread::m_popedJobs;
	if (m_Jobs.size() > 0)
	{
		Job* job = m_Jobs.front();
		m_Jobs.pop();
		*OUT_Job = job;
		return true;
	}
	else
	{
		return false;
	}
}

size_t KernelSchedulerThread::JobCount()
{
	std::lock_guard<std::mutex> guard(m_JobsMutex);
	return m_Jobs.size();
}

size_t KernelSchedulerThread::JobCount_Unsafe() const
{
	return m_Jobs.size();
}

DWORD KernelSchedulerThread::StaticThreadProc(void * p)
{
	return ((KernelSchedulerThread*)p)->ThreadProc();
}

bool KernelSchedulerThread::PopReadyWorker(KernelWorkerThread ** OUT_worker)
{
	std::lock_guard<std::mutex> guard(m_readyWorkersMutex);
	if (m_readyWorkers.size() > 0)
	{
		KernelWorkerThread* worker = m_readyWorkers.front();
		m_readyWorkers.pop();
		*OUT_worker = worker;
		return true;
	}
	else
	{
		return false;
	}
}

std::atomic_uint KernelSchedulerThread::m_pushedJobs = 0;
std::atomic_uint KernelSchedulerThread::m_popedJobs = 0;