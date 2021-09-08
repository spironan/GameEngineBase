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
	virtual Component& CopyComponent(Component const& comp) override;

	/*********************************************************************************//*!
	\brief
	 Getters
	*//**********************************************************************************/
	bool IsPrefab();
	bool IsPrefabDirty();
	engine::Entity GetPrefabReference();
	engine::Entity GetHeadReference();
	bool GetIsShownInEditor();
	/*********************************************************************************//*!
	\brief
	 Setters
	*//**********************************************************************************/
	void SetPrefabDirty(bool pd) ;
	void SetIsPrefab(bool ip);
	void SetHead(Entity head);
	void SetShownInEditor(bool isShown);
	/*********************************************************************************//*!
	\brief    
	 Unregister the old prefab reference 
	 and register the new prefab reference
	\param    reference
	
	*//**********************************************************************************/
	void SetPrefabReference(Entity reference,Entity head);
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
	bool m_isShown_inEditor = true;

	engine::Entity m_prefabReference = 0;
	engine::Entity m_headReference = 0;
	//custom icon

	//
};

};