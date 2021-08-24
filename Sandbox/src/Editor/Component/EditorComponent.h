#pragma once
#include "Engine/ECS/Component.h"
#include <rttr/type>
class EditorComponent : public engine::Component
{
public:
	/*********************************************************************************//*!
	\brief    
	 Default Constructors
	*//**********************************************************************************/
	EditorComponent() = default;
	EditorComponent(const EditorComponent&) = default;
	EditorComponent(EditorComponent&&) = default;
	EditorComponent& operator=(EditorComponent const&) = default;
	EditorComponent& operator=(EditorComponent&&) = default;
	virtual ~EditorComponent() override = default;
	
	/*********************************************************************************//*!
	\brief
	 Getters
	*//**********************************************************************************/
	bool IsPrefab()const { return m_isPrefab; };
	bool IsPrefabDirty()const { return m_isPrefab_Dirty; };

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
	bool m_isPrefab;
	bool m_isPrefab_Dirty;

	//custom icon

	//
};
