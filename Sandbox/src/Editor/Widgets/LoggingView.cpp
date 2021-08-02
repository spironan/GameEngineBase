/************************************************************************************//*!
\file          LoggingView.cpp
\author        Leong Jun Xiang, junxiang.leong , 390007920
\par           email: junxiang.leong\@digipen.edu
\date          July 26, 2021
\brief         Logging System/UI for Editor 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/

#include "LoggingView.h"
#include "Engine/Core/LogCallbackSink.h"
#include "Utility/Hash.h"
#include "Engine/Asset/AssetsManager.h"

#include <imgui.h>
#include <functional>
#include <Windows.h>
#include <shellapi.h>
#include <filesystem>

#include <iostream>
std::deque<engine::utility::StringHash::size_type> LoggingView::s_messages;
std::unordered_map<engine::utility::StringHash::size_type, LoggingView::MessageData> LoggingView::s_messageCollection;
bool LoggingView::s_newItemAdded = false;
bool LoggingView::s_paused = false;
/**
 * \brief 
 *	default constructor 
 * (should only be called in the editor file)
 */
LoggingView::LoggingView()
{
	CallbackSink_mt::SubscribeToSink(AddItem);
}
/**
 * \brief 
 * displaying of UI
 * 
 * \param active
 * 
 */
void LoggingView::Show(bool* active)
{
	static float imageSize = 50.0f;
	bool interacted = false;
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
	ImVec2 textSize = ImGui::CalcTextSize("a");
	size_t textCount = static_cast<size_t>(std::floorf( ((ImGui::GetContentRegionAvail().x - imageSize)) / (textSize.x) * (imageSize / textSize.y - 1)));
	std::string msg_processor;
	msg_processor.resize(textCount +10);
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
						ImGui::PushID(iter->first);
						ImGui::BeginGroup();
						if (ImGui::Selectable("##Item", false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(0, imageSize)) &&
							ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							ShellExecuteA(NULL, "open", iter->second.filename.c_str(), NULL, NULL, SW_SHOW);
						}
						ImGui::SameLine();
						switch (iter->second.type)
						{
						case 0://trace
							ImGui::PushStyleColor(ImGuiCol_Text, { 0.2f,0.5f,0.2f,1 });
							ImGui::Image(reinterpret_cast<ImTextureID>(engine::TextureDatabase::GetTexture("Ouroboros_Log_Icon_Black").id), { imageSize,imageSize });
							break;
						case 1://debug
							ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f,0.5f,0.5f,1 });
							ImGui::Image(reinterpret_cast<ImTextureID>(engine::TextureDatabase::GetTexture("Ouroboros_Log_Icon_Black").id), { imageSize,imageSize });
							break;
						case 2://info
							ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,1,1 });
							ImGui::Image(reinterpret_cast<ImTextureID>(engine::TextureDatabase::GetTexture("Ouroboros_Log_Icon_Black").id), { imageSize,imageSize });
							break;
						case 3://warn
							ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,0,1 }); 
							ImGui::Image(reinterpret_cast<ImTextureID>(engine::TextureDatabase::GetTexture("Ouroboros_Warning_Icon_Yellow").id), { imageSize,imageSize });
							break;
						case 4://err
							ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f,0,0,1 }); 
							ImGui::Image(reinterpret_cast<ImTextureID>(engine::TextureDatabase::GetTexture("Ouroboros_Error_Icon_Red").id), { imageSize,imageSize });
							break;
						case 5://critical
							ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f,0.5f,1,1 });
							ImGui::Image(reinterpret_cast<ImTextureID>(engine::TextureDatabase::GetTexture("Ouroboros_Error_Icon_Red").id), { imageSize,imageSize });
							break;
						}
						ImGui::SameLine();
						//Log Messages UI

						if (ImGui::IsItemHovered())
							interacted = true;
						ImGui::SameLine();

						if (iter->second.msg.size() > textCount)
						{
							msg_processor = iter->second.msg.substr(0, textCount);
							msg_processor += "...";
							ImGui::TextWrapped(msg_processor.c_str());
						}
						else
							ImGui::TextWrapped(iter->second.msg.c_str());
						ImGui::PopStyleColor();
						ImGui::Text("Count: %d \t\t File:: %s", iter->second.count,iter->second.filename.c_str());
						ImGui::EndGroup();
						ImGui::PopID();
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
					auto& item = s_messageCollection[s_messages[i]];
					switch (item.type)
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
					//LogMessages UI
					{
						ImGui::PushID(i);
						ImGui::BeginGroup();
						if (ImGui::Selectable("##Item", false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(0, 0)) &&
							ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							ShellExecuteA(NULL, "open", item.filename.c_str(), NULL, NULL, SW_SHOWNORMAL);
						}
						if (ImGui::IsItemHovered())
							interacted = true;
						ImGui::SameLine();
						ImGui::Text(item.msg.c_str());
						ImGui::PopStyleColor();
						ImGui::EndGroup();
						ImGui::PopID();
					}
				}
			}
		}
		if (s_newItemAdded && !interacted)
		{
			s_newItemAdded = false;
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
		}	
	}
	ImGui::EndChild();
	ImGui::End();
}
/*********************************************************************************//*!
\brief    A callback function to attatch to the spdlog's callback sink
 
\param    str
			the formatted message from spdlog
\param    type
			the logging type
\param    filename
			the file where the log came from

*//**********************************************************************************/
void LoggingView::AddItem(const std::string& str,char type,const std::string& filename)
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

	if (s_messageCollection.find(hash) == s_messageCollection.end())
	{
		s_messageCollection[hash] = { 1,type ,str,filename};
	}
	else
		s_messageCollection[hash].count += 1;
}

