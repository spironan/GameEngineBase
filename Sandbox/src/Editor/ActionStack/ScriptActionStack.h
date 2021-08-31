#pragma once
#include "ActionBehaviour.h"
#include "Engine/Scripting/ScriptInfo.h"
class ScriptActionStack : public ActionBehaviour
{
public:
	void undo();
	void redo();

private:
	
	engine::ScriptFieldInfo undoData;
	engine::ScriptFieldInfo redoData;

};

