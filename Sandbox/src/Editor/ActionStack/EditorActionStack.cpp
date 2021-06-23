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
	for (ActionBehaviour* ac : s_actionDeque)
	{
		delete ac;
	}
}
void ActionStack::UpdateStack()
{
	if (ImGui::IsKeyPressed((int)engine::KeyCode::Z) && ImGui::GetIO().KeyCtrl)
	{
		UndoStep();
	}
	if (ImGui::IsKeyPressed((int)engine::KeyCode::Y) && ImGui::GetIO().KeyCtrl)
	{
		RedoStep();
	}
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


