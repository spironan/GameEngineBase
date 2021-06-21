#pragma once
#include <deque>
#include <functional>

#include "Engine/Memory/BufferAllocator.h"

class ActionStack
{
public:
	ActionStack() { s_dequeIter = s_actionDeque.begin(); s_dequeDataIter = s_actionDequeData.begin(); }

	static void UpdateStack();


	/**
	* this is a helper function to storing the previous state of the item before editing
	*/
	static void AddNewAction(std::function<void(void*)> fnc, void* data);

	template<typename T>
	static T* AllocateInBuffer(const T&);
public:
private:
	//action deque
	static int s_currentBuffer;
	static std::deque<void*>::iterator s_dequeDataIter;
	static std::deque<std::function<void(void*)>>::iterator s_dequeIter;
	static engine::BufferAllocator s_actionBufferAllocator[2];
	static std::deque <void*> s_actionDequeData;
	static std::deque <std::function<void(void*)>> s_actionDeque;
};

template<typename T>
T* ActionStack::AllocateInBuffer(const T& item)
{
	T* ptr;
	if ( (s_actionBufferAllocator[s_currentBuffer].GetRemainingSize() - sizeof(T)) <= 0)
	{
		s_currentBuffer != s_currentBuffer;//swap buffer between 0 and 1
		s_actionBufferAllocator[s_currentBuffer].Clear();//clear the other buffer before using

		//clear data

		s_actionDequeData.erase(s_actionDequeData.begin(), s_dequeDataIter);//clear the dequedata
		s_actionDeque.erase(s_actionDeque.begin(), s_dequeIter);//clear the deque
		
		s_dequeDataIter = s_actionDequeData.begin() + (s_actionDequeData.size() - 1);
		s_dequeIter = s_actionDeque.begin() + (s_actionDeque.size() - 1);
		//
		ptr = s_actionBufferAllocator[s_currentBuffer].New<T>(item);
	}
	else
		ptr = s_actionBufferAllocator[s_currentBuffer].New<T>(item);

	return ptr;
}
