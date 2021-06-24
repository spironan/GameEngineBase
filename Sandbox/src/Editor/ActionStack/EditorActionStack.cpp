//engine
#include "Engine/Memory/MemoryManager.h"
#include "Engine/Core/KeyCode.h"
//sandbox
#include "EditorActionStack.h"
#include "../WarningView.h"
//global
#include <imgui.h>
#include <iterator>//std::distance
//static vars
/*containers*/
std::deque <ActionBehaviour*>				ActionStack::s_actionDeque;

/*buffer vars*/
size_t ActionStack::s_currentBuffer = 0;
size_t ActionStack::s_undoCount = 0;
size_t ActionStack::s_maxHistoryStored = 200;


ActionStack::~ActionStack()
{
	ClearAll();
}
void ActionStack::UpdateStack()
{
	if (ImGui::IsKeyPressed((int)engine::KeyCode::Z) && ImGui::GetIO().KeyCtrl)
		UndoStep();
	if (ImGui::IsKeyPressed((int)engine::KeyCode::Y) && ImGui::GetIO().KeyCtrl)
		RedoStep();
	if (ImGui::Begin("Action Stack"))
	{
		ImGui::BeginChild("##ActionStackChild", { 0,ImGui::GetWindowHeight() * 0.8f }, true);
		size_t undoned_idx = s_actionDeque.size() - s_undoCount;
		for (size_t i = 0; i < s_actionDeque.size(); ++i)
		{
			ImGui::Separator();
			if (i >= undoned_idx)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,0,1 });
				ImGui::TextWrapped(s_actionDeque[i]->m_description.c_str());
				ImGui::PopStyleColor();
			}
			else
				ImGui::TextWrapped(s_actionDeque[i]->m_description.c_str());
			ImGui::Separator();
		}

		if (!s_undoCount && !ImGui::IsWindowHovered())//if undo count is 0
			ImGui::SetScrollHereY();
		ImGui::EndChild();

		if (ImGui::Button("Undo Action")) UndoStep();
		ImGui::SameLine();
		if (ImGui::Button("Redo Action")) RedoStep();
		ImGui::NewLine();
		if (ImGui::Button("Clear History")) ClearAll();
	}
	ImGui::End();
}

void ActionStack::UndoStep()
{
	if (s_actionDeque.size() <= s_undoCount)
	{
		WarningView::DisplayWarning("End of Undo Stack");
		return;
	}
	++s_undoCount;
	ActionBehaviour* ab = *(s_actionDeque.begin() + (s_actionDeque.size() - s_undoCount));
	ab->undo();
}
void ActionStack::RedoStep()
{
	if (s_undoCount == 0)
	{
		WarningView::DisplayWarning("You have reach the most recent action");
		return;
	}
	ActionBehaviour* ab = *(s_actionDeque.begin() + (s_actionDeque.size() - s_undoCount));
	ab->redo();
	--s_undoCount;
}

void ActionStack::ClearAll()
{
	for (ActionBehaviour* ac : s_actionDeque)
	{
		delete ac;
	}
	s_actionDeque.clear();
	s_undoCount = 0;
}

void ActionStack::AllocateInBuffer(ActionBehaviour* item)
{
	//allocating after undoing will clear whatever is infront
	while (s_undoCount)
	{
		ActionBehaviour* ab = s_actionDeque.back();
		delete ab;
		s_actionDeque.pop_back();
		--s_undoCount;
	}
	//resize when too big
	if (s_actionDeque.size() > s_maxHistoryStored)
	{
		for (size_t i = s_maxHistoryStored / 2; i > 0; --i)
		{
			ActionBehaviour* ab = s_actionDeque.front();
			delete ab;
			s_actionDeque.pop_front();
		}
	}
	s_actionDeque.emplace_back(item);
}


