#pragma once

#include "Component.h"
//#include "Engine/Scene/SceneManager.h"
namespace engine
{
	class WorldManager;
	template<typename Type = engine::WorldManager>
    class ComponentMk2 : public Component
    {
	public:
		using Manager = Type;
		ComponentMk2() = delete;
		ComponentMk2(Entity entity, bool active = true) : Component{ entity,active } {}

		template<typename T>
		T& GetComponent()
		{
			return Manager::GetActiveWorld().GetComponent<T>(m_entity);
		}
    };

}