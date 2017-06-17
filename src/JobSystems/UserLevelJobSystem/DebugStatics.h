#pragma once

class DebugStatics
{
public:

	static void AddMessage(const char* msg);
	static std::string& GetMsg();
	static void ClearMessage();
private:

	static std::string m_Message;
	static std::mutex m_messageMutex;
};

