#include "stdafx.h"
#include "DebugStatics.h"

void DebugStatics::AddMessage(const char * msg)
{
	std::lock_guard<std::mutex> guard(m_messageMutex);
	m_Message.append(msg);
}

std::string & DebugStatics::GetMsg()
{
	return m_Message;
}

void DebugStatics::ClearMessage()
{
	std::lock_guard<std::mutex> guard(m_messageMutex);
	m_Message = "";
}


std::string DebugStatics::m_Message = std::string();
std::mutex DebugStatics::m_messageMutex;
