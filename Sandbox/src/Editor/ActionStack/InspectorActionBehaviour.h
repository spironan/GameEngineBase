#pragma once
#include "ActionBehaviour.h"
#include "Engine/ECS/ECS_Utility.h"
#include "Engine/Transform/Transform3D.h"
#include "Engine/Prefab/EditorComponent.h"
#include "Engine/ECS/GameObject.h"
#include <rttr/property.h>
#include <rttr/variant.h>
template <typename Component>
class InspectorActionBehaviour : public ActionBehaviour
{
public:
	InspectorActionBehaviour(const std::string& str, engine::Entity ett, const rttr::property& p, rttr::variant& _undo, rttr::variant& _redo, bool dirty)
		:ActionBehaviour{ str }, item(ett), prop(p), undoData(_undo), redoData(_redo), m_dirty(dirty)
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
		auto& go = static_cast<engine::GameObject>(item);
		go.GetComponent<engine::EditorComponent>().SetPrefabDirty(m_dirty);
	}
	void redo()override
	{
		prop.set_value(static_cast<engine::GameObject>(item).GetComponent<Component>(), redoData);
		auto& go = static_cast<engine::GameObject>(item);
		go.GetComponent<engine::EditorComponent>().SetPrefabDirty(false);
	}

private:
	engine::Entity item;
	rttr::property prop;
	rttr::variant undoData;
	rttr::variant redoData;
	bool m_dirty;
};


