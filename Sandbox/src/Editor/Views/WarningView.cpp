#include "WarningView.h"

#include <imgui.h>

//pesudo timers
int WarningView::s_counter = 0;
const int WarningView::s_max_counter = 300;
//warning variables
bool WarningView::s_ShowWarning = false;
std::string WarningView::s_WarningMessage;
float WarningView::s_position[2];
WarningView::DisplayType WarningView::s_dtype = WarningView::DisplayType::DISPLAY_LOG;
void WarningView::Show()
{
	if (s_ShowWarning)
	{
		ImGui::SetNextWindowPos({s_position[0],s_position[1]});
		ImGui::SetNextWindowBgAlpha(0);
		ImGui::Begin("WarningView", 0, ImGuiWindowFlags_NoDecoration| ImGuiWindowFlags_NoDocking |ImGuiWindowFlags_AlwaysAutoResize| ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoInputs);
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		switch (s_dtype)
		{
		case DisplayType::DISPLAY_LOG:
			ImGui::TextColored({ 1,1,1,1 }, "%s", s_WarningMessage.c_str());
			break;
		case DisplayType::DISPLAY_WARNING:
			ImGui::TextColored({ 1,0.5,0.5,1 }, "%s", s_WarningMessage.c_str());
			break;
		case DisplayType::DISPLAY_ERROR:
			ImGui::TextColored({ 1,0,0,1 }, "%s", s_WarningMessage.c_str());
			break;
		default:
			break;
		}
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
		ImGui::End();
		--s_counter;
		if (!s_counter)
			s_ShowWarning = false;
	}
}
void WarningView::DisplayWarning(DisplayType type ,const std::string& str)
{
	s_dtype = type;
	s_WarningMessage = str;
	s_ShowWarning = true;
	s_counter = s_max_counter;
	s_position[0] = ImGui::GetMousePos().x;
	s_position[1] = ImGui::GetMousePos().y;

}
