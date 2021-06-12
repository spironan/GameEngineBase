#pragma once
#include <deque>
#include <string>
//#include <varargs.h>
#include <stdio.h>
class LoggingView
{
public:

	void Show();

	static void AddLoggingMsg(const char* fmt, ...);
	static std::deque<std::string> s_messages;

};


//#define LOG_TRACE(n,...)              LoggingView::AddLoggingMsg(n,__VA_ARGS__);



