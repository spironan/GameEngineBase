#pragma once
#include <deque>
#include <functional>
#include <iterator>//std::distance

#include "Engine/Memory/BufferAllocator.h"

class ActionStack
{
public:
	ActionStack() { s_actionSwapIter = s_actionDeque.begin(); }

	static void UpdateStack();

	/**
	* this is a helper function to storing the previous state of the item before editing
	*/
	static void AddNewAction(std::function<void(void*)> fnc, void* data);
	static void AddNewAction(std::function<void(void*)> fnc, void* data, void* redoData);
	static void AppendRedoData(void* data);
	template<typename T>
	static T* AllocateInBuffer(const T&);

private:
	static void UndoStep();
	static void RedoStep();
public:
private:
	struct ActionCommand
	{
		std::function<void(void*)> fnc;
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
	while (s_undoCount)
	{
		ActionCommand& ac = s_actionDeque.back();
		delete ac.data;
		delete ac.redoData;
		s_actionDeque.pop_back();
		--s_undoCount;
	}
	//resize when too big
	if (s_actionDeque.size() > s_maxHistoryStored)
	{
		for (size_t i = s_maxHistoryStored / 2; i > 0; --i)
		{
			ActionCommand& ac = s_actionDeque.front();
			delete ac.data;
			delete ac.redoData;
			s_actionDeque.pop_front();
		}
	}
	T* ptr = new T(item);
	return ptr;
}
