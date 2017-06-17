#include "stdafx.h"
#include "Scheduler.h"
#include "DebugStatics.h"
#include "ThreadingUtility.h"

Scheduler::Scheduler()
	: m_numOfProcessors(ThreadingUtility::GetNumberOfProcessors()), m_pushedJobs(0)
{
}

KernelSchedulerThread * Scheduler::getMinSizedJobQueue()
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


Scheduler::~Scheduler()
{
}

Scheduler * Scheduler::GetInstance()
{
	if (!m_instance)
		m_instance = new Scheduler();

	return m_instance;
}

void Scheduler::Initialize()
{
	//Init scheduler threads:
	m_numOfSchedulerThreads = m_numOfProcessors - 1;
#if JOBSYSTEM_DEBUG_LOG
	char b[512];int n = 0;
	DebugStatics::AddMessage([this,&b,&n]()->char*{ n = sprintf(b, "-- [%d] KernelSchedulerThreads will be initialized...\n", m_numOfSchedulerThreads); return b; }());
#endif
	for (int i = 0; i < m_numOfSchedulerThreads; i++)
	{
		m_schedulerThreads[i].Initialize(this);
		//m_schedulerThreads[i].m_ProcessorAffinity = pow(2, i);
	}


}

bool Scheduler::PushJob(Job * Job)
{
	std::lock_guard<std::mutex> guard(m_JobsMutex);
	++m_pushedJobs;
	return getMinSizedJobQueue()->PushJob(Job);
	////Spinlock may be faster: main thread is only pushing Jobs
	//std::lock_guard<std::mutex> guard(m_JobsMutex);
	//m_Jobs.push(Job);

	//return true;
}
bool Scheduler::AreAllSchedulerThreadsReadyForProcessing() const
{
	bool areAllReady = true;
	for (int i = 0; i < m_numOfSchedulerThreads; i++)
	{
		areAllReady = areAllReady && m_schedulerThreads[i].IsReadyForProcessing();
	}
	return areAllReady;
}
//
//bool Scheduler::PopJob(Job ** OUT_Job)
//{
//	std::lock_guard<std::mutex> guard(m_JobsMutex);
//	if (m_Jobs.size() > 0)
//	{
//		Job* Job = m_Jobs.front();
//		m_Jobs.pop();
//		*OUT_Job = Job;
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//size_t Scheduler::JobCount()
//{
//	std::lock_guard<std::mutex> guard(m_JobsMutex);
//	return m_Jobs.size();
//}

Scheduler* Scheduler::m_instance = nullptr;