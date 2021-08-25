#pragma once
#include "Engine/ECS/System.h"
#include <vector>
#include <unordered_map>
class EditorComponent;
class EditorComponentSystem final : public engine::System
{
public:
	/*-----------------------------------------------------------------------------*/
	/* Class Specific Type Aliases                                                 */
	/*-----------------------------------------------------------------------------*/
	using value_type = EditorComponent;
	using value_reference = value_type&;
	using value_const_reference = value_type const&;

	using container_type = std::vector<value_type>;
	using container_reference = container_type&;
	using container_iterator = container_type::iterator;
	using container_const_iterator = container_type::const_iterator;

	/*-----------------------------------------------------------------------------*/
	/* Explicit Constructor                                                        */
	/*-----------------------------------------------------------------------------*/
	explicit EditorComponentSystem(engine::ECS_Manager& _ECS_Manager) : System{ _ECS_Manager } {};

	/*********************************************************************************//*!
	\brief    
	 update loop
	*//**********************************************************************************/
	void Update();
	/*********************************************************************************//*!
	\brief
	 Update All entity using the prefab when marked for update
	 should only be called when interacting with editor UI
	\param
	 target item to update
	*//**********************************************************************************/
	void UpdatedPrefab(value_reference);
	/*********************************************************************************//*!
	\brief    
	 When creating a new object with prefab register this entity into m_prefabUsers
	\param    
	 takes in EditorComponent 
	*//**********************************************************************************/
	void RegisterNewUser(value_reference);
	/*********************************************************************************//*!
	\brief    
	 When breaking from prefab / deleting
	\param    
	 the reference to EditorComponent
	*//**********************************************************************************/
	void UnregisterUser(value_reference);

private:
	std::map<engine::Entity, std::vector<engine::Entity> > m_prefabUsers;
};
