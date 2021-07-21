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
#include "Utility/Hash.h"

class LoggingView
{
public:
	LoggingView();
	void Show(bool* active);
	
	//static void AddLoggingMsg(const char* fmt, ...);
protected:
	static void AddItem(const std::string&,char);
private:
	struct MessageData
	{
		int count;
		char type;
		std::string msg;
	};
	static std::deque<engine::utility::StringHash::size_type> s_messages;
	static std::unordered_map<engine::utility::StringHash::size_type, MessageData> s_messageCollection;
	static bool s_newItemAdded;
	static bool s_paused;

	bool m_collapse_similar = false;
};


