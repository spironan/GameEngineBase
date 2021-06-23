#pragma once

class ActionBehaviour
{
public:
	ActionBehaviour() {};
	virtual ~ActionBehaviour() = 0;
	virtual void undo() = 0;
	virtual void redo() = 0;

};

