#include "stdafx.h"
//#include "FiberScheduler.h"
//#include "DebugStatics.h"
//#include "ThreadingUtility.h"
//
//FiberScheduler::FiberScheduler()
//	: m_numOfProcessors(ThreadingUtility::GetNumberOfProcessors())
//{
//	
//}
//
//FiberScheduler::~FiberScheduler()
//{
//}
//
//FiberScheduler * FiberScheduler::GetInstance()
//{
//	if (!m_instance)
//	{
//		std::unique_lock<std::mutex> guard(m_initInstanceMutex);
//		if (!m_instance)
//		{
//			m_instance = new FiberScheduler();
//		}
//	}
//
//	return m_instance;
//}
//
//void FiberScheduler::Initialize()
//{
//	//Init scheduler threads (suspended):
//	m_numOfSchedulerThreads = m_numOfProcessors - 1;
//#if JOBSYSTEM_DEBUG_LOG
//	char b[512];int n = 0;
//	DebugStatics::AddMessage([this, &b, &n]()->char* { n = sprintf(b, "-- [%d] FiberSchedulerThreads (Kernel Thread) will be initialized...\n", m_numOfSchedulerThreads); return b; }());
//#endif
//	for (int i = 0; i < m_numOfSchedulerThreads; i++)
//	{
//		m_schedulerThreads[i].Initialize(this);
//		//m_schedulerThreads[i].m_ProcessorAffinity = pow(2, i);
//	}
//
//	m_assistentWorkerThread.Initialize(this);
//
//	//Start scheduler threads:
//	/*for (int i = 0; i < m_numOfSchedulerThreads; i++)
//	{
//		if (ResumeThread(m_schedulerThreads[i].m_Thread) == -1){ printf("Error %d", GetLastError()); }
//	}*/
//}
//
//bool FiberScheduler::PushJob(Job * Job)
//{
//	return getMinSizedJobQueue()->PushJob(Job);
//}
//
//bool FiberScheduler::PushExpensiveJobFiber(Fiber * fiber)
//{
//	m_assistentWorkerThread.PushExpensiveFiber(fiber);
//	return true;
//}
//
//bool FiberScheduler::AreAllSchedulerThreadsReadyForProcessing() const
//{
//	bool areAllReady = true;
//	areAllReady = m_assistentWorkerThread.IsReadyForProcessing();
//	for (int i = 0; i < m_numOfSchedulerThreads; i++)
//	{
//		areAllReady = areAllReady && m_schedulerThreads[i].IsReadyForProcessing();
//	}
//	return areAllReady;
//}
//
//Fiber * FiberScheduler::GetFiberByPointer(void * fiberPointer)
//{
//	for (int i = 0; i < m_numOfSchedulerThreads; i++)
//	{
//		Fiber* result = m_schedulerThreads[i].GetFiberByPointer(fiberPointer);
//		if (result != nullptr)
//			return result;
//	}
//	return nullptr;
//}
//
//Fiber * FiberScheduler::GetCurrentExecutingFiberBySchedulerThread(FiberSchedulerThread * schedulerThread)
//{
//	for (int i = 0; i < m_numOfSchedulerThreads; ++i)
//	{
//
//	}
//	return nullptr;
//}
//
//FiberSchedulerThread * FiberScheduler::getMinSizedJobQueue()
//{
//	int minIdx = 0;
//	for (int i = 0; i < m_numOfSchedulerThreads; i++)
//	{
//		if (i + 1 <= m_numOfSchedulerThreads - 1)
//		{
//			if (m_schedulerThreads[i].JobCount() > m_schedulerThreads[i + 1].JobCount())
//			{
//				minIdx = i + 1;
//			}
//		}
//	}
//	return &m_schedulerThreads[minIdx];
//}
//
//FiberScheduler* FiberScheduler::m_instance = nullptr;
//std::mutex FiberScheduler::m_initInstanceMutex;