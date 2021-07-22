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
#include "Engine/Core/LogCallbackSink.h"
#include "Utility/Hash.h"

#include <imgui.h>
#include <functional>



std::deque<engine::utility::StringHash::size_type> LoggingView::s_messages;
std::unordered_map<engine::utility::StringHash::size_type, LoggingView::MessageData> LoggingView::s_messageCollection;
bool LoggingView::s_newItemAdded = false;
bool LoggingView::s_paused = false;
LoggingView::LoggingView()
{
	CallbackSink_mt::SubscribeToSink(AddItem);
}

void LoggingView::Show(bool* active)
{
	
	ImGui::Begin("Logger", active,ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::MenuItem("Clear"))
		{
			s_messages.resize(0);
		}
		if (ImGui::MenuItem((s_paused)?"UnPause" : "Pause", NULL, s_paused))
		{
			s_paused = !s_paused;
		}
		if (ImGui::MenuItem((m_collapse_similar)? "Expand" : "Collapse", NULL, m_collapse_similar, true))
		{
			m_collapse_similar = !m_collapse_similar;
		}
		ImGui::EndMenuBar();
	}

	//draw ui here
	if (ImGui::BeginChild("LogView Child"))
	{
		if (m_collapse_similar)
		{
			ImGuiListClipper clipper;
			clipper.Begin(static_cast<int>(s_messageCollection.size()),35.0f);
			int counter = 0;
			while (clipper.Step())
			{
				for (auto& iter = s_messageCollection.begin(); iter != s_messageCollection.end(); ++iter,++counter)
				{
					if (counter >= clipper.DisplayStart)
					{
						switch (iter->second.type)
						{
						case 0:
							ImGui::PushStyleColor(ImGuiCol_Text, { 0.2f,0.5f,0.2f,1 });break;
						case 1:
							ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f,0.5f,0.5f,1 });break;
						case 2:
							ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,1,1 });break;
						case 3:
							ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,0,1 }); break;
						case 4:
							ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f,0,0,1 }); break;
						case 5:
							ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f,0.5f,1,1 }); break;
						}
						ImGui::TextWrapped(iter->second.msg.c_str());
						ImGui::PopStyleColor();
						ImGui::Text(std::to_string(iter->second.count).c_str());
						ImGui::Separator();
						if (counter >= clipper.DisplayEnd)
							break;
					}
					else
						continue;
				}
			}
		}
		else
		{
			ImGuiListClipper clipper;
			clipper.Begin(static_cast<int>(s_messages.size()));
			while (clipper.Step())
			{
				int distance = clipper.DisplayEnd - clipper.DisplayStart;
				int start = static_cast<int>(s_messages.size()) - 1 - clipper.DisplayStart;
				for (int i = start; i > start - distance; --i)
				{
					switch (s_messageCollection[s_messages[i]].type)
					{
					case 0:
						ImGui::PushStyleColor(ImGuiCol_Text, { 0.2f,0.5f,0.2f,1 }); break;
					case 1:
						ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f,0.5f,0.5f,1 }); break;
					case 2:
						ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,1,1 }); break;
					case 3:
						ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,0,1 }); break;
					case 4:
						ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f,0,0,1 }); break;
					case 5:
						ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f,0.5f,1,1 }); break;
					}
					ImGui::Text(s_messageCollection[s_messages[i]].msg.c_str());
					ImGui::PopStyleColor();
				}
			}
		}
		if (s_newItemAdded)
		{
			s_newItemAdded = false;
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
		}	
	}
	ImGui::EndChild();
	ImGui::End();
}

void LoggingView::AddItem(const std::string& str,char type)
{
	if (s_paused)
		return;
	s_newItemAdded = true;
	if (s_messages.size() > 250)
	{
		s_messages.resize(200);
	}
	//when added new message set the scroll bar to the newest
	engine::utility::StringHash::size_type hash = engine::utility::StringHash::GenerateFNV1aHash(str);
	//emplace infomation about the logs
	s_messages.emplace_front(hash);
	//to track the count for the logs

	if(s_messageCollection.find(hash) == s_messageCollection.end())
		s_messageCollection[hash] = { 0,type,str };
	else
		s_messageCollection[hash].count += 1;
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
