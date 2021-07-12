/*****************************************************************//**
 * \file   LoggingView.cpp
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

#include "LoggingView.h"

#include <imgui.h>
#include <sstream>
#include <iostream>

// required to obtain the ostringstream to read its message
#include <Engine/Core/Log.h>
#include "Engine/Core/LogCallbackSink.h"

#include <functional>
#include <sstream>

std::deque<std::string> LoggingView::s_messages;
std::unordered_map<std::string, int> LoggingView::s_messagesRepeat;
bool LoggingView::s_newItemAdded = false;

LoggingView::LoggingView()
{
	std::function<void(const std::string&)> item = AddItem;
	CallbackSink_mt::SubscribeToSink(item);
}

void LoggingView::Show()
{
	
	ImGui::Begin("Logger",0,ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::MenuItem("Clear"))
		{
			s_messages.resize(0);
		}
		if (ImGui::MenuItem("Pause",NULL,m_paused))
		{
			m_paused = !m_paused;
		}
		if (ImGui::MenuItem("Collapsed",NULL,m_collapse_similar))
		{
			m_collapse_similar = !m_collapse_similar;
		}
		ImGui::EndMenuBar();
	}

	//draw ui here
	if (m_collapse_similar)
	{
		for (auto& item : s_messagesRepeat)
		{
			ImGui::Separator();
			ImGui::TextWrapped(item.first.c_str());
			ImGui::Text(std::to_string(item.second).c_str());
			ImGui::Separator();
		}
	}
	else
	{
		for(int i = static_cast<int>(s_messages.size()) - 1 ; i > 0; --i)
			ImGui::Text(s_messages[i].c_str());
	}

	if (s_newItemAdded)
	{
		s_newItemAdded = false;
		ImGui::SetScrollY(ImGui::GetScrollMaxY());
	}

	//if paused do not process string
	if (m_paused)
	{
		ImGui::End();
		return;
		
	}
	

	ImGui::End();
}

void LoggingView::AddItem(const std::string& str)
{
	s_newItemAdded = true;
	if (s_messages.size() > 250)
	{
		s_messages.resize(200);
	}
	//when added new message set the scroll bar to the newest

	//emplace infomation about the logs
	s_messages.emplace_front(str);
	//to track the count for the logs
	++s_messagesRepeat[str];
}

//this is now unused
//void LoggingView::AddLoggingMsg(const char* fmt, ...)
//{
//	char buf[1024];
//	va_list vargs;
//	va_start(vargs, fmt);
//	vsnprintf(buf, sizeof(buf), fmt, vargs);
//	buf[sizeof(buf) - 1] = '\0';
//	s_messages.emplace_back(buf);
//}
