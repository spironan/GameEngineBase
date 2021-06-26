/*****************************************************************//**
 * \file   LoggingView.h
 * \brief  Logs the console outputs into a window
 *		   Logs the console output from scripts
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include <deque>
#include <string>
#include <stdio.h>
class LoggingView
{
public:

	void Show();

	static void AddLoggingMsg(const char* fmt, ...);
	static std::deque<std::string> s_messages;
private:
	bool m_paused = false;
};


