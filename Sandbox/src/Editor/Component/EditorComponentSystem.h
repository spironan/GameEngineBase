#pragma once
#include "Engine/ECS/System.h"
#include "EditorComponent.h"
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
	 update loop used to check if anything is dirty
	*//**********************************************************************************/
	void Update();
	/*********************************************************************************//*!
	\brief
	 Update All entity using the prefab when marked for update
	*//**********************************************************************************/
	void UpdateAllPrefabUsers();

};
