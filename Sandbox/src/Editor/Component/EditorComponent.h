#pragma once
#include "Engine/ECS/Component.h"
#include "Engine/ECS/ECS_Utility.h"
#include <rttr/type>
class EditorComponent : public engine::Component
{
public:
	/*********************************************************************************//*!
	\brief    
	 Default Constructors
	*//**********************************************************************************/
	EditorComponent() = delete;
	EditorComponent(const EditorComponent&) = default;
	EditorComponent(EditorComponent&&) = default;
	EditorComponent& operator=(EditorComponent const&) = default;
	EditorComponent& operator=(EditorComponent&&) = default;
	virtual ~EditorComponent() override = default;
	
	EditorComponent(engine::Entity entity, bool active) :Component{ entity,active } {};
	/*********************************************************************************//*!
	\brief
	 Getters
	*//**********************************************************************************/
	bool IsPrefab() { return m_isPrefab; };
	bool IsPrefabDirty() { return m_isPrefab_Dirty; };
	engine::Entity GetPrefabReference() { return m_prefabReference; };
	/*********************************************************************************//*!
	\brief
	 Setters
	*//**********************************************************************************/
	void SetPrefab(bool p) { m_isPrefab = p; };
	void SetPrefabDirty(bool pd) { m_isPrefab_Dirty = pd; };
	void SetPrefabReference(engine::Entity e) { m_prefabReference = e; };
	/*********************************************************************************//*
	\brief    
	 Will not show any prefab icons if broken off
	*//**********************************************************************************/
	void BreakOffFromPrefab();
	/*********************************************************************************//*!
	\brief    
	 Update Main Prefab which will also update all entities
	*//**********************************************************************************/
	void UpdatePrefab();
	
	RTTR_ENABLE();
private:

private:
	bool m_isPrefab = false;
	bool m_isPrefab_Dirty = false;

	engine::Entity m_prefabReference = 0;
	//custom icon

	//
};
