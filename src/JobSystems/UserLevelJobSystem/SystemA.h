#pragma once

class Game;

class SystemA
{
public:
	SystemA();
	~SystemA();
	
	Game* m_Game;
	
	int Update();

	//DEBUG:
	static std::atomic_uint m_JobExecCount;

private:



};