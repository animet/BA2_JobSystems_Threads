#include "stdafx.h"
#include "SystemB.h"

#include "JobManager.h"
//#include "UMSScheduler.h"
#include "FiberScheduler.h"
#include "Scheduler.h"
#include "KernelThreadScheduler.h"
#include "Game.h"

SystemB::SystemB()
{
}


SystemB::~SystemB()
{
}

int SystemB::Update()
{
	Job* job = JobManager::GetInstance()->CreateJob();
	job->m_Callback = [](Job* job) {

		++SystemB::m_JobExecCount;

		auto start_time = std::chrono::high_resolution_clock::now();
		//Costs: cheap
		int arr[10000];
		/*for (int i = 0; i < 100; i++)
		{
			arr[i] = i + 1;
		}*/
		//Costs: expensive
		std::string str;
		for (int i = 0; i < 1000; i++)
		{
			str.append("a");
		}
		auto end_time = std::chrono::high_resolution_clock::now();
		auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	};

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

	return 1;
}

std::atomic_uint SystemB::m_JobExecCount = 0;