#pragma once

class Game;

class ExpensiveSystemX
{
public:
	ExpensiveSystemX();
	~ExpensiveSystemX();

	Game* m_Game;

	int Update();

	//DEBUG:
	static std::atomic_uint m_JobExecCount;
};

