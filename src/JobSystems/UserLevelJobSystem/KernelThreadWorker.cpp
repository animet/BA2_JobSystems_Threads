#include "stdafx.h"
#include "KernelThreadWorker.h"
#include "KernelThreadScheduler.h"
#include "JobManager.h"
#include "Game.h"

#define KERNELTHREAD_WORKER_STACKSIZE 512 * 1024 //512KB

KernelThreadWorker::KernelThreadWorker()
	: m_isReadyForProcessing(false)
{
}

KernelThreadWorker::~KernelThreadWorker()
{
}

void KernelThreadWorker::Initialize(KernelThreadScheduler * scheduler)
{
	m_scheduler = scheduler;

	m_Thread = CreateThread(NULL, KERNELTHREAD_WORKER_STACKSIZE, StaticThreadProc, this, 0, &m_ThreadId); //Creates and starts thread. Suspends the thread: (CREATE_SUSPENDED)
}

DWORD KernelThreadWorker::ThreadProc()
{
	printf("The worker thread with the id '%d' is ready.\n", GetCurrentThreadId());

#if JOBSYSTEM_DEBUG_LOG
	char b[512];int n = 0;
	DebugStatics::AddMessage([this, &b, &n]()->char* { n = sprintf(b, "-- [%d] KernelWorkerThreads will be initialized for the KernelSchedulerThread with the Id '%d'...\n", MAX_WORKERTHREADS_PER_SCHEDULER_THREAD, GetCurrentThreadId()); return b; }());
#endif
	m_isReadyForProcessing = true;

	std::unique_lock<std::mutex> guard(m_scheduleMutex);
	m_scheduleCondition.wait(guard);
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

			Sleep(1);
			//m_scheduleCondition.wait(guard); : The reason that some jobs are not completed is the signal in combination with job count
			//TODO: Steal Work VS working signal
		}

		Job* job = nullptr;
		if (PopJob(&job))
		{
			job->m_Callback(job);
			JobManager::GetInstance()->FinishJob(job);
		}
	}
	return 0;
}

bool KernelThreadWorker::IsReadyForProcessing() const
{
	return m_isReadyForProcessing;
}

void KernelThreadWorker::Activate()
{
	m_scheduleCondition.notify_one();
}

Job * KernelThreadWorker::FrontJob()
{
	std::lock_guard<std::mutex> guard(m_JobsMutex);
	return m_Jobs.front();
}

bool KernelThreadWorker::PushJob(Job * Job)
{
	//Spinlock may be faster: main thread is only pushing Jobs
	std::lock_guard<std::mutex> guard(m_JobsMutex);
	//++KernelThreadWorker::m_pushedJobs;
	m_Jobs.push(Job);

	return true;
}

bool KernelThreadWorker::PopJob(Job ** OUT_Job)
{
	std::lock_guard<std::mutex> guard(m_JobsMutex);
	//++KernelThreadWorker::m_popedJobs;
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

size_t KernelThreadWorker::JobCount()
{
	std::lock_guard<std::mutex> guard(m_JobsMutex);
	return m_Jobs.size();
}

size_t KernelThreadWorker::JobCount_Unsafe() const
{
	return m_Jobs.size();
}

DWORD KernelThreadWorker::StaticThreadProc(void * p)
{
	return ((KernelThreadWorker*)p)->ThreadProc();
}

//std::atomic_uint KernelThreadWorker::m_pushedJobs = 0;
//std::atomic_uint KernelThreadWorker::m_popedJobs = 0;