#pragma once

class Game;

class SystemB
{
public:
	SystemB();
	~SystemB();

	Game* m_Game;

	int Update();

	//DEBUG:
	static std::atomic_uint m_JobExecCount;
};

