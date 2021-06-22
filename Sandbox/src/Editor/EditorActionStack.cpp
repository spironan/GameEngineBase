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
/*iters*/
std::deque<ActionStack::ActionCommand>::iterator ActionStack::s_actionSwapIter;



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
/**
 * \brief
 *		is a helper function to storing the previous state of the item before editing
 *		deleting of data is handled by editor no need to delete in fnc
 * \param fnc
 *		this is the function pointer of the function
 *		please do not bind the function before this
 * \param data
 *		any kind of data that is use by the function
 */
void ActionStack::AddNewAction(std::function<void(void*)> fnc, void* data)
{
	ActionStack::s_actionDeque.emplace_back(ActionCommand{fnc, data,nullptr});
}
/**
 * \brief
 *		is a helper function to storing the previous state of the item before editing
 *		deleting of data is handled by editor no need to delete in fnc
 * \param fnc
 *		this is the function pointer of the function
 *		please do not bind the function before this !!!!!!
 * \param data
 *		any kind of data that is use by the function
 * \param redoData
 *		any kind of data that is use by the function(does the opp of data)
 */
void ActionStack::AddNewAction(std::function<void(void*)> fnc, void* data,void* redoData)
{
	ActionStack::s_actionDeque.emplace_back(ActionCommand{ fnc, data,redoData });
}

/**
 * \brief 
 *		Appends the redoData to the newest on the deque
 * \param data
 *		the redo data which uses the same function  
 */
void ActionStack::AppendRedoData(void* data)
{
	s_actionDeque.back().redoData = data;
}



