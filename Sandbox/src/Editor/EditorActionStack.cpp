#include "EditorActionStack.h"
#include "Engine/Memory/MemoryManager.h"
#include <imgui.h>
#include "Engine/Core/KeyCode.h"
#include "WarningView.h"
std::deque <std::function<void(void*)>> ActionStack::s_actionDeque;
std::deque <void*>						ActionStack::s_actionDequeData;
engine::BufferAllocator ActionStack::s_actionBufferAllocator[2]{ engine::MemoryManager::NewBufferAllocator(128, 8) ,engine::MemoryManager::NewBufferAllocator(128, 8) };//2kb
int ActionStack::s_currentBuffer = 0;

std::deque<void*>::iterator ActionStack::s_dequeDataIter;
std::deque<std::function<void(void*)>>::iterator ActionStack::s_dequeIter;


void ActionStack::UpdateStack()
{
	if (ImGui::IsKeyPressed((int)engine::KeyCode::Z) && ImGui::GetIO().KeyCtrl)
	{
		if (s_actionDeque.size())
		{
			(*s_actionDeque.rbegin())(nullptr);
			s_actionBufferAllocator[s_currentBuffer].FreeToPtr((engine::BufferAllocator::PtrInt)(*s_actionDequeData.rbegin()));//clear the used data
			s_actionDeque.pop_back();
			s_actionDequeData.pop_back();
		}
		else
		{
			WarningView::DisplayWarning("There is no history queued");
		}
	}
}

/**
 * \brief
 *		is a helper function to storing the previous state of the item before editing
 *		deleting of data is handled by editor no need to delete in fnc
 * \param fnc
 *		this is the function pointer of the function
 *		please do not bind the function before this !!!!!! i will bind it for u
 * \param data
 *		any kind of data that is use by the function (this will be used for binding)
 */
void ActionStack::AddNewAction(std::function<void(void*)> fnc, void* data)
{
	ActionStack::s_actionDequeData.emplace_back(data);
	ActionStack::s_actionDeque.emplace_back(std::bind(fnc,data));
}



