#include "stdafx.h"
#include "DependencySystemH.h"

#include "JobManager.h"
//#include "UMSScheduler.h"
#include "FiberScheduler.h"
#include "Scheduler.h"
#include "KernelThreadScheduler.h"
#include "Game.h"

DependencySystemH::DependencySystemH()
{
}

DependencySystemH::~DependencySystemH()
{
}

int DependencySystemH::Update()
{
	int jobCounter = 1;
	Job* job = JobManager::GetInstance()->CreateJob();
	job->m_Callback = [](Job* job) {
		int a = 1 % 5;
	};

	jobCounter += 9;
	for (int i = 0; i < 9; ++i)
	{
		Job* child = JobManager::GetInstance()->CreateChildJob(job);
		child->m_Callback = [](Job* job) {
			int b = 100 / 4;
		};

		switch (m_Game->CurrentUserLevelImpl)
		{
		case UserLevelImplementation::FIBER:
			FiberScheduler::GetInstance()->PushJob(child);
			break;
		case UserLevelImplementation::KERNEL_W_KERNELWORKERS:
			Scheduler::GetInstance()->PushJob(child);
			break;
		case UserLevelImplementation::KERNELTHREAD:
			KernelThreadScheduler::GetInstance()->PushJob(child);
			break;
		default:
			assert(false);
			break;
		}
	}

	switch (m_Game->CurrentUserLevelImpl)
	{
	case UserLevelImplementation::FIBER:
		FiberScheduler::GetInstance()->PushJob(job);
		break;
	case UserLevelImplementation::KERNEL_W_KERNELWORKERS:
		Scheduler::GetInstance()->PushJob(job);
		break;
	case UserLevelImplementation::KERNELTHREAD:
		KernelThreadScheduler::GetInstance()->PushJob(job);
		break;
	default:
		assert(false);
		break;
	}

	JobManager::GetInstance()->WaitForJob(job, 0);

	return jobCounter;
}

std::atomic_uint DependencySystemH::m_JobExecCount = 0;