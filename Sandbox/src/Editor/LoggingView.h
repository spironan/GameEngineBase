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
#include <unordered_map>
#include <string>
class LoggingView
{
public:
	LoggingView();
	void Show();
	
	//static void AddLoggingMsg(const char* fmt, ...);
protected:
	static void AddItem(const std::string&);
private:
	static std::deque<std::string> s_messages;
	static std::unordered_map<std::string, int> s_messagesRepeat;
	static bool s_newItemAdded;

	bool m_collapse_similar = false;
	bool m_paused = false;
};


