#include "stdafx.h"
#include "KernelThreadScheduler.h"
#include "DebugStatics.h"
#include "ThreadingUtility.h"


KernelThreadScheduler::KernelThreadScheduler()
	: m_numOfProcessors(ThreadingUtility::GetNumberOfProcessors()) //, m_pushedJobs(0)
{
}

KernelThreadWorker * KernelThreadScheduler::getMinSizedJobQueue()
{
	int minIdx = 0;
	for (int i = 0; i < m_numOfWorkerThreads; i++)
	{
		if (i + 1 <= m_numOfWorkerThreads - 1)
		{
			if (m_workerThreads[i].JobCount_Unsafe() > m_workerThreads[i + 1].JobCount_Unsafe())
			{
				minIdx = i + 1;
			}
		}
	}
	return &m_workerThreads[minIdx];
}


KernelThreadScheduler::~KernelThreadScheduler()
{
}

KernelThreadScheduler * KernelThreadScheduler::GetInstance()
{
	if (!m_instance)
	{
		std::lock_guard<std::mutex> guard(m_initInstanceMutex);
		if (!m_instance)
		{
			m_instance = new KernelThreadScheduler();
		}
	}

	return m_instance;
}

void KernelThreadScheduler::Initialize()
{
	//Init worker threads:
	m_numOfWorkerThreads = m_numOfProcessors - 1;
#if JOBSYSTEM_DEBUG_LOG
	char b[512];int n = 0;
	DebugStatics::AddMessage([this, &b, &n]()->char* { n = sprintf(b, "-- [%d] KernelThreadWorkers will be initialized...\n", m_numOfWorkerThreads); return b; }());
#endif
	for (int i = 0; i < m_numOfWorkerThreads; i++)
	{
		m_workerThreads[i].Initialize(this);
		//m_schedulerThreads[i].m_ProcessorAffinity = pow(2, i);
	}


}

void KernelThreadScheduler::ActivateWorkers()
{
	for (int i = 0; i < m_numOfWorkerThreads; i++)
	{
		m_workerThreads[i].Activate();
	}
}

bool KernelThreadScheduler::PushJob(Job * Job)
{
	////Spinlock may be faster: main thread is only pushing Jobs
	std::lock_guard<std::mutex> guard(m_JobsMutex);
	//++m_pushedJobs;
	return getMinSizedJobQueue()->PushJob(Job);
}

bool KernelThreadScheduler::AreAllWorkerThreadsReadyForProcessing() const
{
	bool areAllReady = true;
	for (int i = 0; i < m_numOfWorkerThreads; i++)
	{
		areAllReady = areAllReady && m_workerThreads[i].IsReadyForProcessing();
	}
	return areAllReady;
}

KernelThreadScheduler* KernelThreadScheduler::m_instance = nullptr;
std::mutex KernelThreadScheduler::m_initInstanceMutex;