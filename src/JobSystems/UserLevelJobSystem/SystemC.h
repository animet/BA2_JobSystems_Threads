#pragma once

class Game;

class SystemC
{
public:
	SystemC();
	~SystemC();

	Game* m_Game;

	int Update();

	//DEBUG:
	static std::atomic_uint m_JobExecCount;
};

