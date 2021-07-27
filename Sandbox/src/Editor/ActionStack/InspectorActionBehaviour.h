#pragma once
#include "ActionBehaviour.h"
#include "Engine/ECS/ECS_Utility.h"
#include "Engine/Transform/Transform3D.h"

#include <rttr/property.h>
#include <rttr/variant.h>
template <typename Component>
class InspectorActionBehaviour : public ActionBehaviour
{
public:
	InspectorActionBehaviour(const std::string& str,engine::Entity ett,const rttr::property& p, rttr::variant& _undo,rttr::variant& _redo) 
		:ActionBehaviour{str}, item(ett), prop(p), undoData(_undo),redoData(_redo) 
	{

	}
/*********************************************************************************//*!
\brief    
	function for cleaning the data assigned
	some data structures might be too complex
	that just doing a delete on a void* will not work
*//**********************************************************************************/
	virtual ~InspectorActionBehaviour()override
	{
			undoData.clear();
			redoData.clear();
	}
	void undo()override
	{
		prop.set_value(static_cast<engine::GameObject>(item).GetComponent<Component>(), undoData);
	}
	void redo()override
	{
		prop.set_value(static_cast<engine::GameObject>(item).GetComponent<Component>(), redoData);
	}

private:
	engine::Entity item;
	rttr::property prop;
	rttr::variant undoData;
	rttr::variant redoData;
};


