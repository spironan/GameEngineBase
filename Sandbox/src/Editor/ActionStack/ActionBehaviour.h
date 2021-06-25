#pragma once
#include <string>
class ActionBehaviour
{
public:
	ActionBehaviour(const std::string& str) :m_description(str){};
	virtual ~ActionBehaviour() = 0;
	virtual void undo() = 0;
	virtual void redo() = 0;
public:
	std::string m_description;
};

