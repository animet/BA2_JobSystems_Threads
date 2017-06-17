#include "stdafx.h"
#include "FiberWorker.h"
#include "Job.h"
#include "JobManager.h"
#include "FiberSchedulerThread.h"

#define FIBER_STACKSIZE 16 * 1024 //16KB

FiberWorker::FiberWorker()
	: m_CurrentJob(nullptr), m_hasWork(false) //, m_AssignedAssistantThread(nullptr) //, m_Fiber(INVALID_HANDLE_VALUE)
{
}


FiberWorker::~FiberWorker()
{
}

void FiberWorker::Initialize(FiberSchedulerThread * schedulerThread)
{
	m_SchedulerThread = schedulerThread;

	m_FiberPointer = CreateFiber(FIBER_STACKSIZE, FiberWorker::StaticProc, this);
	if (m_FiberPointer == NULL) { printf("Error %d", GetLastError()); }
}

void FiberWorker::SetJob(Job * Job)
{
	m_CurrentJob = Job;
	m_hasWork = true;
}

FiberSchedulerThread * FiberWorker::GetFiberSchedulerThread()
{
	return m_SchedulerThread;
}

void FiberWorker::Proc(void * p)
{
	while (true)
	{
		if (m_CurrentJob && m_hasWork)
		{
			m_CurrentJob->m_Callback(m_CurrentJob);
			JobManager::GetInstance()->FinishJob(m_CurrentJob);
		}
		m_hasWork = false;
		m_SchedulerThread->PushReadyFiber(this);

		SwitchToFiber(m_SchedulerThread->m_SchedulerFiberPointer);
	}
}

void FiberWorker::StaticProc(void * p)
{
	((FiberWorker*)p)->Proc(nullptr);
}
