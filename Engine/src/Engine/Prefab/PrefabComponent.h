#pragma once
#include "Engine/ECS/Component.h"

namespace engine
{
	class PrefabComponent :public Component
	{
	public:
		/*********************************************************************************//*!
		\brief
		 Default Constructors
		*//**********************************************************************************/
		PrefabComponent() = delete;
		PrefabComponent(const PrefabComponent&) = default;
		PrefabComponent(PrefabComponent&&) = default;
		PrefabComponent& operator=(PrefabComponent const&) = default;
		PrefabComponent& operator=(PrefabComponent&&) = default;
		virtual ~PrefabComponent() override = default;

		/*********************************************************************************//*!
		\brief    
		 Prefab Component will set the main gameobject to inactive
		*//**********************************************************************************/
		PrefabComponent(Entity entity, bool active);
		
	public:
		bool m_RootNode = false;
		

	};

}
