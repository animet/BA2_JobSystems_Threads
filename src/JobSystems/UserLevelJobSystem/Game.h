#pragma once
#include "SystemA.h"
#include "SystemB.h"
#include "SystemC.h"
#include "ExpensiveSystemX.h"
#include "ExpensiveSystemY.h"
#include "ExpensiveSystemZ.h"
#include "DependencySystemH.h"

class Game
{
public:
	Game();
	~Game();

	UserLevelImplementation CurrentUserLevelImpl;

	void Initialize();

	void Start();

	void Shutdown();

	static std::shared_timed_mutex m_FinishMutex;
	static std::atomic_bool m_FinishedAllJobs;

private:

	SystemA m_systemA;
	SystemB m_systemB;
	SystemC m_systemC;

	ExpensiveSystemX m_expensiveSystemX;
	ExpensiveSystemY m_expensiveSystemY;
	ExpensiveSystemZ m_expensiveSystemZ;

	DependencySystemH m_dependencySystemH;
};

