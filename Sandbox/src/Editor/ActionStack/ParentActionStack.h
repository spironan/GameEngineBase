#pragma once
#include "ActionBehaviour.h"
#include "Engine/ECS/ECS_Utility.h"//entity
#include "Engine/ECS/GameObject.h"//gameobject
engine::Entity;
class ParentActionStack:public ActionBehaviour
{
public:
	ParentActionStack(engine::Entity id,engine::Entity prev_ParentID , engine::Entity curr_Parentid):
		ActionBehaviour(engine::GameObject(id).Name() + " parented to " + engine::GameObject(curr_Parentid).Name()),
		m_id(id),m_prev_parentID(prev_ParentID),m_curr_parentID(curr_Parentid)
	{
	};
	~ParentActionStack()
	{

	};
	void undo()override
	{
		engine::GameObject(m_prev_parentID).AddChild(m_id);
	}
	void redo()override
	{
		engine::GameObject(m_curr_parentID).AddChild(m_id);
	}
private:
	engine::Entity m_id;
	engine::Entity m_prev_parentID;
	engine::Entity m_curr_parentID;

};
