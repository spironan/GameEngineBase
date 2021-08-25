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
	
	/*********************************************************************************//*!
	\brief
	 Getters
	*//**********************************************************************************/
	bool IsPrefab() { return m_isPrefab; };
	bool IsPrefabDirty() { return m_isPrefab_Dirty; };
	engine::Entity GetPrefabReference() { return m_prefabReference; };
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
