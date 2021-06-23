#pragma once
#include <deque>
#include <functional>
#include <iterator>//std::distance

#include "Engine/Memory/BufferAllocator.h"

class ActionStack
{
public:
	ActionStack() {}
	~ActionStack();

	static void UpdateStack();

	template<typename T>
	static T* AllocateInBuffer(const T&);


	static void DefaultCleanUpFunction(void* data) { delete data; }

	template <typename T>
	static void AddNewAction(std::function<void(void*)> fnc, T& data, T& redoData, std::function<void(void*)> cleanup = DefaultCleanUpFunction);


private:
	static void UndoStep();
	static void RedoStep();
public:
private:
	struct ActionCommand
	{
		std::function<void(void*)> fnc;
		std::function<void(void*)> cleanup;
		void* data;
		void* redoData;
	};
	//action deque
	static size_t s_currentBuffer;
	static size_t s_undoCount;



	static size_t s_maxHistoryStored;

	static std::deque<ActionCommand>::iterator s_actionSwapIter;


	static std::deque <ActionCommand> s_actionDeque;
};

template<typename T>
T* ActionStack::AllocateInBuffer(const T& item)
{
	//allocating after undoing will clear whatever is infront
	std::cout << sizeof(T) << std::endl;
	while (s_undoCount)
	{
		ActionCommand& ac = s_actionDeque.back();
		ac.cleanup(ac.data);
		ac.cleanup(ac.redoData);
		s_actionDeque.pop_back();
		--s_undoCount;
	}
	//resize when too big
	if (s_actionDeque.size() > s_maxHistoryStored)
	{
		for (size_t i = s_maxHistoryStored / 2; i > 0; --i)
		{
			ActionCommand& ac = s_actionDeque.front();
			ac.cleanup(ac.data);
			ac.cleanup(ac.redoData);
			s_actionDeque.pop_front();
		}
	}
	T* ptr = new T(item);
	return ptr;
}
/**
 * \brief
 *		is a helper function to storing the previous state of the item before editing
 *		deleting of data is handled by editor no need to delete in fnc
 * \param fnc
 *		this is the function pointer of the function
 *		please do not bind the function before this !!!!!!
 * \param data
 *		any kind of data that is use by the function(do not allocate on heap)
 *		the function allocates it for u
 * \param redoData
 *		any kind of data that is use by the function(does the opp of data)
 *		(do not allocate on heap)
 *		the function allocates it for u
 */
template <typename T>
void ActionStack::AddNewAction(std::function<void(void*)> fnc, T& undoData, T& redoData, std::function<void(void*)> cleanup)
{
	void* undo = AllocateInBuffer(undoData);
	void* redo = AllocateInBuffer(redoData);
	ActionStack::s_actionDeque.emplace_back(ActionCommand{ fnc,cleanup, undo,redo });
}