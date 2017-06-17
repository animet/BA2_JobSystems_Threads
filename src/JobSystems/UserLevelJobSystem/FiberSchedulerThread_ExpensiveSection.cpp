#include "stdafx.h"
//#include "FiberSchedulerThread.h"
//#include "DebugStatics.h"
//#include "FiberScheduler.h"
//
//#define FIBER_SCHEDULER_THREAD_STACKSIZE 512 * 1024 //512KB
//
//FiberSchedulerThread::FiberSchedulerThread()
//	: m_isReadyForProcessing(false)
//{
//}
//
//
//FiberSchedulerThread::~FiberSchedulerThread()
//{
//}
//
//void FiberSchedulerThread::Initialize(FiberScheduler * scheduler)
//{
//	m_scheduler = scheduler;
//
//	m_Thread = CreateThread(NULL, FIBER_SCHEDULER_THREAD_STACKSIZE, StaticThreadProc, this, 0, &m_ThreadId); //Creates and starts thread. Suspends the thread: (CREATE_SUSPENDED)
//}
//
//DWORD FiberSchedulerThread::ThreadProc()
//{
//	printf("The thread with the id '%d' will be converted to a fiber to schedule other fibers...\n", GetCurrentThreadId());
//
//	m_schedulerFiberPointer = ConvertThreadToFiber(this); //&m_schedulerFiber); //scheduler thread is converted to fiber to schedule other fibers. Members of scheduler fibers are currently not needed
//	if (m_schedulerFiberPointer == NULL) { printf("Error %d", GetLastError()); }
//	//Initialize fibers:
//#if JOBSYSTEM_DEBUG_LOG
//	char b[512];int n = 0;
//	DebugStatics::AddMessage([this, &b, &n]()->char* { n = sprintf(b, "-- [%d] Fibers will be initialized for the FiberSchedulerThread (Kernel Thread) with the Id '%d'...\n", MAX_FIBERS_PER_SCHEDULER_THREAD, GetCurrentThreadId()); return b; }());
//#endif
//	for (int i = 0; i < MAX_FIBERS_PER_SCHEDULER_THREAD; i++)
//	{
//		m_fibers[i].Initialize(this);
//		m_readyFibers.push(&m_fibers[i]);
//	}
//	m_isReadyForProcessing = true;
//
//	std::unique_lock<std::mutex> guard(m_scheduleMutex);
//	while (true)
//	{
//		//https://msdn.microsoft.com/en-us/library/windows/desktop/ms686919(v=vs.85).aspx
//		//Execute ready fibers....
//		//printf("Fiber scheduler thread with the id '%d' is now working...\n", GetCurrentThreadId());
//
//		//1.) Dispatch fibers with work left
//		Fiber* fiberWithWorkLeft = nullptr;
//		if (PopFiberWithWorkLeft(&fiberWithWorkLeft))
//		{
//			SwitchToFiber(fiberWithWorkLeft->m_FiberPointer);
//		}
//
//		//2.) Dispatch new work
//		while (JobCount() <= 0)
//		{
//			m_scheduleCondition.wait(guard); //The signal alone is not enough (race)
//		}
//
//		/*
//		Important: First pop ready worker, then Job!
//		*/
//
//		if (m_readyFibers.size() > 0)
//		{
//			Fiber* fiber = m_readyFibers.front();
//			if (fiber)
//			{
//				Job* Job = nullptr;
//				if (PopJob(&Job))
//				{
//					fiber->SetJob(Job);
//					m_readyFibers.pop();
//					m_currentExecutingFiber = fiber;
//					SwitchToFiber(fiber->m_FiberPointer);
//				}
//				else if (Job == nullptr)
//				{
//					m_readyFibers.push(fiber);
//				}
//			}
//		}
//		
//	}
//	return 0;
//}
//
//bool FiberSchedulerThread::IsReadyForProcessing() const
//{
//	return m_isReadyForProcessing;
//}
//
//void FiberSchedulerThread::PushReadyFiber(Fiber * fiber)
//{
//	//Thought: Do not need to protect access because only one fiber per thread can run at the same time
//	m_readyFibers.push(fiber);
//}
//
//void FiberSchedulerThread::PushExpensiveFiber(Fiber * fiber)
//{
//	//FiberSchedulerThread and AssistantThread will access this queue
//	//std::unique_lock<std::mutex> guard(m_expensiveFibersMutex); //Maybe use spinlock
//	//m_expensiveFibers.push(fiber);
//	m_scheduler->PushExpensiveJobFiber(fiber);
//}
//
//void FiberSchedulerThread::PushFiberWithWorkLeft(Fiber * fiber, void* assistantFiberPointer)
//{
//	std::unique_lock<std::mutex> guard(m_fibersWithWorkLeftMutex);
//	m_fibersWithWorkLeft.push(fiber);
//	//SwitchToFiber(assistantFiberPointer); //Must switch in scope of its assistant thread!
//}
//
//bool FiberSchedulerThread::PushJob(Job * Job)
//{
//	//Spin lock may be faster: main thread is only pushing Jobs
//	std::unique_lock<std::mutex> guard(m_JobsMutex);
//	m_Jobs.push(Job);
//	//m_JobsCondition.notify_one();
//	m_scheduleCondition.notify_one();
//
//	return true;
//}
//
//bool FiberSchedulerThread::PopJob(Job ** OUT_Job)
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
//size_t FiberSchedulerThread::JobCount()
//{
//	std::lock_guard<std::mutex> guard(m_JobsMutex);
//	return m_Jobs.size();
//}
//
//Fiber * FiberSchedulerThread::GetFiberByPointer(void * fiberPointer)
//{
//	for (int i = 0; i < MAX_FIBERS_PER_SCHEDULER_THREAD; ++i)
//	{
//		if (m_fibers[i].m_FiberPointer == fiberPointer)
//			return &m_fibers[i];
//	}
//	return nullptr;
//}
//
//Fiber * FiberSchedulerThread::GetCurrentExecutingFiber()
//{
//	return m_currentExecutingFiber;
//}
//
//DWORD FiberSchedulerThread::StaticThreadProc(void * p)
//{
//	return ((FiberSchedulerThread*)p)->ThreadProc();
//}
//
//bool FiberSchedulerThread::PopFiberWithWorkLeft(Fiber ** OUT_fiber)
//{
//	std::lock_guard<std::mutex> guard(m_fibersWithWorkLeftMutex);
//	if (m_fibersWithWorkLeft.size() > 0)
//	{
//		Fiber* fiber = m_fibersWithWorkLeft.front();
//		m_fibersWithWorkLeft.pop();
//		*OUT_fiber = fiber;
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
