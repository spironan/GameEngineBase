/************************************************************************************//*!
\file          LoggingView.h
\author        Leong Jun Xiang, junxiang.leong , 390007920
\par           email: junxiang.leong\@digipen.edu
\date          July 26, 2021
\brief         Declaration of the Logging System/UI 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
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
	static void AddItem(const std::string&,char,const std::string&);
private:
	struct MessageData
	{
		int count;
		char type;
		std::string msg;
		std::string filename;
	};
	static std::deque<engine::utility::StringHash::size_type> s_messages;
	static std::unordered_map<engine::utility::StringHash::size_type, MessageData> s_messageCollection;
	static bool s_newItemAdded;
	static bool s_paused;

	bool m_collapse_similar = false;
};


