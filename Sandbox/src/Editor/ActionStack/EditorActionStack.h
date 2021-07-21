#pragma once
#include <deque>
#include <functional>
#include <iterator>//std::distance

#include "ActionBehaviour.h"
class ActionStack
{
public:
	ActionStack() {}
	~ActionStack();
	void Show(bool* active);

	static void UpdateStack();
	static void AllocateInBuffer(ActionBehaviour* item);

private:
	static void UndoStep();
	static void RedoStep();
	static void ClearAll();
public:
private:
	//action deque
	static size_t s_currentBuffer;
	static size_t s_undoCount;
	static size_t s_maxHistoryStored;

	static std::deque <ActionBehaviour*> s_actionDeque;

};



