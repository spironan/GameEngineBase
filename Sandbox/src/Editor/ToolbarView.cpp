#include "ToolbarView.h"
#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>
void ToolbarView::Show()
{
	ImGui::Begin("Toolbar",NULL,ImGuiWindowFlags_NoDecoration);
	float w = ImGui::GetWindowWidth();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });
	{
		ImGui::BeginChild("ChildToolbar", { 0,0 });
		ImGui::Button("##Play", { btn_width,btn_height });
		ImGui::SameLine();
		ImGui::Button("##Play2", { btn_width,btn_height });
		ImGui::SameLine();
		ImGui::Button("##Play3", { btn_width,btn_height });
		ImGui::SameLine();
		ImGui::Button("##Play4", { btn_width,btn_height });
		ImGui::EndChild();
		ImGui::SameLine(w * 0.5f - (btn_width * 3 * 0.5f));
	}
	{
		ImGui::BeginChild("ChildToolbar2", { 0,0 });
		ImGui::Button("##2Play", { btn_width,btn_height });
		ImGui::SameLine();
		ImGui::Button("##2Play2", { btn_width,btn_height });
		ImGui::SameLine();
		ImGui::Button("##2Play3", { btn_width,btn_height });
		ImGui::EndChild(); 
	}
	{
		ImGui::SameLine(w - (btn_width * 4));
		ImGui::BeginChild("ChildToolbar3", { 0,0 });
		if (ImGui::Button("Undock", { btn_width,btn_height }))
		{
			docking = !docking;
		}
		ImGui::SameLine();
		ImGui::Button("##3Play2", { btn_width,btn_height });
		ImGui::SameLine();
		ImGui::Button("##3Play3", { btn_width,btn_height });
		ImGui::EndChild();
	}
	ImGui::PopStyleVar();
	ImGui::End();

	auto* window = ImGui::FindWindowByName("Toolbar");
	if (window->DockNode)
	{
		if (docking)
			window->DockNode->LocalFlags = 0;
		else
			window->DockNode->LocalFlags = ImGuiDockNodeFlags_NoTabBar|ImGuiDockNodeFlags_NoResizeY | ImGuiDockNodeFlags_NoDocking;
	}
	else
		ImGui::SetWindowSize(window,{ 500,100 });

}