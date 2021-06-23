//engine
#include "Engine/Memory/MemoryManager.h"
#include "Engine/Core/KeyCode.h"
//sandbox
#include "EditorActionStack.h"
#include "WarningView.h"
//global
#include <imgui.h>
#include <iterator>//std::distance
//static vars

/*containers*/
std::deque <ActionStack::ActionCommand>				ActionStack::s_actionDeque;

/*buffer vars*/
size_t ActionStack::s_currentBuffer = 0;
size_t ActionStack::s_undoCount = 0;
size_t ActionStack::s_maxHistoryStored = 200;


ActionStack::~ActionStack()
{
	for (ActionCommand& ac : s_actionDeque)
	{
		ac.cleanup(ac.data);
		ac.cleanup(ac.redoData);
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
	ActionCommand& ac = *(s_actionDeque.begin() + (s_actionDeque.size() - s_undoCount));
	ac.fnc(ac.data);
}
void ActionStack::RedoStep()
{
	if (s_undoCount == 0)
	{
		WarningView::DisplayWarning("You have reach the most recent action");
		return;
	}
	ActionCommand& ac = *(s_actionDeque.begin() + (s_actionDeque.size() - s_undoCount));
	ac.fnc(ac.redoData);
	--s_undoCount;
}



