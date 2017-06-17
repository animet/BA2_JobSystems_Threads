#include "stdafx.h"
#include "ExpensiveSystemY.h"

#include "JobManager.h"
//#include "UMSScheduler.h"
#include "FiberScheduler.h"
#include "Scheduler.h"
#include "KernelThreadScheduler.h"
#include "Game.h"

#include "ExpensiveSection.h"

ExpensiveSystemY::ExpensiveSystemY()
{
}


ExpensiveSystemY::~ExpensiveSystemY()
{
}



int ExpensiveSystemY::Update()
{
	Job* job = JobManager::GetInstance()->CreateJob();
	job->m_Callback = [](Job* job) {

		++ExpensiveSystemY::m_JobExecCount;
		auto start_time = std::chrono::high_resolution_clock::now();
		
		/*ExpensiveSection::Enter();*/
		 int result = _mkdir("C:\\Temp\\abc");
		 if (result == 0)
		 {
			result = _rmdir("C:\\Temp\\abc");
		 }
		 /*ExpensiveSection::Leave();*/

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

std::atomic_uint ExpensiveSystemY::m_JobExecCount = 0;