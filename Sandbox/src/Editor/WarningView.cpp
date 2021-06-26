#include "WarningView.h"

#include <imgui.h>

//pesudo timers
int WarningView::s_counter = 0;
const int WarningView::s_max_counter = 300;
//warning variables
bool WarningView::s_ShowWarning = false;
std::string WarningView::s_WarningMessage;
float WarningView::s_position[2];
void WarningView::Show()
{
	if (s_ShowWarning)
	{
		ImGui::SetNextWindowPos({s_position[0],s_position[1]});
		ImGui::SetNextWindowBgAlpha(0);
		ImGui::Begin("WarningView", 0, ImGuiWindowFlags_NoDecoration| ImGuiWindowFlags_NoDocking |ImGuiWindowFlags_AlwaysAutoResize| ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoInputs);
		ImGui::TextColored({ 1,0.5,0.5,1 }, "%s", s_WarningMessage.c_str());
		ImGui::End();
		--s_counter;
		if (!s_counter)
			s_ShowWarning = false;
	}
}
void WarningView::DisplayWarning(const std::string& str)
{
	s_WarningMessage = str;
	s_ShowWarning = true;
	s_counter = s_max_counter;
	s_position[0] = ImGui::GetMousePos().x;
	s_position[1] = ImGui::GetMousePos().y;

}
