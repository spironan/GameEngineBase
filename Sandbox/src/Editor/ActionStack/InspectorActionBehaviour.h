#pragma once
#include "ActionBehaviour.h"
#include "Engine.h"

#include <rttr/property.h>
#include <rttr/variant.h>
class InspectorActionBehaviour : public ActionBehaviour
{
public:
	InspectorActionBehaviour(const std::string& str,engine::Entity ett,const rttr::property& p, rttr::variant& _undo,rttr::variant& _redo) 
		:ActionBehaviour{str}, item(ett), prop(p), undoData(_undo),redoData(_redo) {}
	virtual ~InspectorActionBehaviour()override;
	void undo()override;
	void redo()override;

private:
	engine::Entity item;
	rttr::property prop;
	rttr::variant undoData;
	rttr::variant redoData;
};

