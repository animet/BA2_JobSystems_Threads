#include "stdafx.h"
#include "SystemA.h"

#include "JobManager.h"
//#include "UMSScheduler.h"
#include "FiberScheduler.h"
#include "Scheduler.h"
#include "KernelThreadScheduler.h"
#include "Game.h"
#include "ExpensiveSection.h"


SystemA::SystemA()
{
}


SystemA::~SystemA()
{
}

int SystemA::Update()
{
	Job* job = JobManager::GetInstance()->CreateJob();
	job->m_Callback = [](Job* job) {

		++SystemA::m_JobExecCount;
		auto start_time = std::chrono::high_resolution_clock::now();
		//Costs:cheap
		/*long l = 0;
		for (int i = 0; i < 100; i++)
		{
			l += i + 100;
		}*/
		
		//Costs: Medium
		for (int i = 0; i < 10000; i++)
		{
			float x = sqrt(i + 10) / i * 3;
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

std::atomic_uint SystemA::m_JobExecCount = 0;