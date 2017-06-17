#include "stdafx.h"
#include "FiberScheduler.h"
#include "DebugStatics.h"
#include "ThreadingUtility.h"

FiberScheduler::FiberScheduler()
	: m_numOfProcessors(ThreadingUtility::GetNumberOfProcessors())
{
	
}

FiberScheduler::~FiberScheduler()
{
}

FiberScheduler * FiberScheduler::GetInstance()
{
	if (!m_instance)
	{
		std::lock_guard<std::mutex> guard(m_initInstanceMutex);
		if (!m_instance)
		{
			m_instance = new FiberScheduler();
		}
	}

	return m_instance;
}

void FiberScheduler::Initialize()
{
	//Init scheduler threads (suspended):
	m_numOfSchedulerThreads = m_numOfProcessors - 1;
#if JOBSYSTEM_DEBUG_LOG
	char b[512];int n = 0;
	DebugStatics::AddMessage([this, &b, &n]()->char* { n = sprintf(b, "-- [%d] FiberSchedulerThreads (Kernel Thread) will be initialized...\n", m_numOfSchedulerThreads); return b; }());
#endif
	for (int i = 0; i < m_numOfSchedulerThreads; i++)
	{
		m_schedulerThreads[i].Initialize(this);
		//m_schedulerThreads[i].m_ProcessorAffinity = pow(2, i);
	}
}

void FiberScheduler::ActivateSchedulerThreads()
{
	for (int i = 0; i < m_numOfSchedulerThreads; i++)
	{
		m_schedulerThreads[i].Activate();
	}
}

bool FiberScheduler::PushJob(Job * Job)
{
	return getMinSizedJobQueue()->PushJob(Job);
}


bool FiberScheduler::AreAllSchedulerThreadsReadyForProcessing() const
{
	bool areAllReady = true;
	for (int i = 0; i < m_numOfSchedulerThreads; i++)
	{
		areAllReady = areAllReady && m_schedulerThreads[i].IsReadyForProcessing();
	}
	return areAllReady;
}

FiberWorker * FiberScheduler::GetFiberByPointer(void * fiberPointer)
{
	for (int i = 0; i < m_numOfSchedulerThreads; i++)
	{
		FiberWorker* result = m_schedulerThreads[i].GetFiberByPointer(fiberPointer);
		if (result != nullptr)
			return result;
	}
	return nullptr;
}

FiberSchedulerThread * FiberScheduler::getMinSizedJobQueue()
{
	int minIdx = 0;
	for (int i = 0; i < m_numOfSchedulerThreads; i++)
	{
		if (i + 1 <= m_numOfSchedulerThreads - 1)
		{
			if (m_schedulerThreads[i].JobCount_Unsafe() > m_schedulerThreads[i + 1].JobCount_Unsafe())
			{
				minIdx = i + 1;
			}
		}
	}
	return &m_schedulerThreads[minIdx];
}

FiberScheduler* FiberScheduler::m_instance = nullptr;
std::mutex FiberScheduler::m_initInstanceMutex;