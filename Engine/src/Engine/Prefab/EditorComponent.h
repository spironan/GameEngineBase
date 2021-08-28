#pragma once
#include "Engine/ECS/Component.h"
#include "Engine/ECS/ECS_Utility.h"
#include <rttr/type>
namespace engine
{

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
	
	EditorComponent(engine::Entity entity, bool active);
	/*********************************************************************************//*!
	\brief
	 Getters
	*//**********************************************************************************/
	bool IsPrefab();
	bool IsPrefabDirty();
	engine::Entity GetPrefabReference();
	/*********************************************************************************//*!
	\brief
	 Setters
	*//**********************************************************************************/
	void SetPrefabDirty(bool pd) ;
	/*********************************************************************************//*!
	\brief    
	 Unregister the old prefab reference 
	 and register the new prefab reference
	\param    reference
	
	*//**********************************************************************************/
	void SetPrefabReference(Entity reference);
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

};