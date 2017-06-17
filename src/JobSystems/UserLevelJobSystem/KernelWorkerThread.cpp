#include "stdafx.h"
#include "KernelWorkerThread.h"
#include "JobManager.h"
#include "KernelSchedulerThread.h"
#include "Game.h"

#define KERNEL_WORKER_THREAD_STACKSIZE 64 * 1024 //10KB


KernelWorkerThread::KernelWorkerThread()
	: m_JobReady(0)
{
}


KernelWorkerThread::~KernelWorkerThread()
{
}

void KernelWorkerThread::Initialize(KernelSchedulerThread * schedulerThread)
{
	m_SchedulerThread = schedulerThread;

	m_Thread = CreateThread(NULL, KERNEL_WORKER_THREAD_STACKSIZE, StaticThreadProc, this, 0, &m_ThreadId); //Creates (and starts) thread. Suspends the thread: (CREATE_SUSPENDED)
}

void KernelWorkerThread::SetJob(Job * Job)
{
	m_CurrentJob = Job;
	++m_JobReady;
	if (m_JobReady > 1)
	{
		assert(false);
	}
	m_workCondition.notify_one();
}

DWORD KernelWorkerThread::ThreadProc()
{
	std::unique_lock<std::mutex> guard(m_workMutex);
	while (true)
	{
		while (m_JobReady <= 0)
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
		if (m_JobReady > 1)
		{
			assert(false);
		}
		
		if (m_CurrentJob)
		{
			m_CurrentJob->m_Callback(m_CurrentJob);
			--m_JobReady;
			JobManager::GetInstance()->FinishJob(m_CurrentJob);
		}
		else
		{
			assert(false);
		}
		m_SchedulerThread->PushReadyWorker(this);
		//Sleep(0); //like the fiber
	}
	return 0;
}

DWORD KernelWorkerThread::StaticThreadProc(void * p)
{
	return ((KernelWorkerThread*)p)->ThreadProc();
}
