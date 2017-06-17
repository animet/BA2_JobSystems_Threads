#include "stdafx.h"
#include "Game.h"

#include "DebugStatics.h"
#include "FiberScheduler.h"
#include "Scheduler.h"
#include "KernelThreadScheduler.h"
#include "JobManager.h"

Game::Game()
{
}


Game::~Game()
{
}

void Game::Initialize()
{
	m_systemA.m_Game = this;
	m_systemB.m_Game = this;
	m_systemC.m_Game = this;
	m_expensiveSystemX.m_Game = this;
	m_expensiveSystemY.m_Game = this;
	m_expensiveSystemZ.m_Game = this;
	m_dependencySystemH.m_Game = this;

#ifdef USE_STEPS_FOR_MEMORY_ANALYSIS
	printf("Thread creation begins...");
	getchar();
#endif // USE_STEPS_FOR_MEMORY_ANALYSIS

	

	switch (CurrentUserLevelImpl)
	{
	case UserLevelImplementation::FIBER:
		FiberScheduler::GetInstance()->Initialize();
		break;
	case UserLevelImplementation::UMS:
		assert(false);
		break;
	case UserLevelImplementation::KERNEL_W_KERNELWORKERS:
		assert(false);
		break;
	case UserLevelImplementation::KERNELTHREAD:
		KernelThreadScheduler::GetInstance()->Initialize();
		break;
	default:
		break;
	}

	bool waitForThreads = true;
	while (waitForThreads)
	{
		switch (CurrentUserLevelImpl)
		{
		case UserLevelImplementation::FIBER:
			if (FiberScheduler::GetInstance()->AreAllSchedulerThreadsReadyForProcessing())
			{
				waitForThreads = false;
			}
			break;
		case UserLevelImplementation::UMS:
			assert(false);
			break;
		case UserLevelImplementation::KERNEL_W_KERNELWORKERS:
			assert(false);
			break;
		case UserLevelImplementation::KERNELTHREAD:
			if (KernelThreadScheduler::GetInstance()->AreAllWorkerThreadsReadyForProcessing())
			{
				waitForThreads = false;
			}
			break;
		default:
			break;
		}
		Sleep(50);
	}

#ifdef USE_STEPS_FOR_MEMORY_ANALYSIS
	printf("Thread creation finished.");
	getchar();
#endif
	
	//...
}

void Game::Start()
{
	
#if JOBSYSTEM_DEBUG_LOG
	printf("%s", DebugStatics::GetMsg().c_str());
	DebugStatics::ClearMessage();
#endif 


	auto start_time = std::chrono::high_resolution_clock::now();

	switch (CurrentUserLevelImpl)
	{
	case UserLevelImplementation::FIBER:
		FiberScheduler::GetInstance()->ActivateSchedulerThreads();
		break;
	case UserLevelImplementation::UMS:
		assert(false);
		break;
	case UserLevelImplementation::KERNEL_W_KERNELWORKERS:
		assert(false);
		break;
	case UserLevelImplementation::KERNELTHREAD:
		KernelThreadScheduler::GetInstance()->ActivateWorkers();
		break;
	default:
		break;
	}

	
	//Add and process Jobs:
	u_int JobCounter = 0;

	for (int i = 0; i < 100000; ++i) //1000
	{
		 JobCounter += m_systemA.Update();
		 JobCounter += m_systemB.Update();
		 JobCounter += m_systemC.Update();
		JobCounter += m_expensiveSystemX.Update();
		JobCounter += m_expensiveSystemY.Update();
		JobCounter += m_expensiveSystemZ.Update();

		//JobCounter += m_dependencySystemH.Update();
	}

#if JOBSYSTEM_DEBUG_LOG
	char b[512];int n = 0;
	DebugStatics::AddMessage([this, &b, &n, &JobCounter]()->char* { n = sprintf(b, "-- [%d] Jobs were created and added for dispatching.\n", JobCounter); return b; }());
#endif 
	auto end_time = std::chrono::high_resolution_clock::now();
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	//printf("\nGame Systems: Took %.3f ms to add all Jobs\n", static_cast<float>(microseconds) / static_cast<float>(1000.0f));

	//Wait until all Jobs are finished
#if JOBSYSTEM_DEBUG_LOG
	printf("%s", DebugStatics::GetMsg().c_str());
	DebugStatics::ClearMessage();
#endif 
	
	while (true)
	{
		if (JobManager::GetInstance()->GetFinishedJobs() == JobCounter)
		{
			m_FinishedAllJobs = true;

			auto end_time_all = std::chrono::high_resolution_clock::now();
			auto microseconds_all = std::chrono::duration_cast<std::chrono::microseconds>(end_time_all - start_time).count();
			printf("\nGame Systems: Took %.3f ms to complete all Jobs\n", static_cast<float>(microseconds_all) / static_cast<float>(1000.0f));

#ifdef USE_STEPS_FOR_MEMORY_ANALYSIS
			getch();
#endif // USE_STEPS_FOR_MEMORY_ANALYSIS

			break;
		}
		Sleep(0);
	}
	
}

void Game::Shutdown()
{
}

std::shared_timed_mutex Game::m_FinishMutex;
std::atomic_bool Game::m_FinishedAllJobs = false;