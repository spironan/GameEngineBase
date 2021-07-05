/*****************************************************************//**
\file       EntityManager.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui@digipen.edu
\date       2/7/2021
\brief  
EntityManager handles entities and creation and deletion of entitites.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once

#include "ECS_Utility.h"
#include "Engine/Core/Assert.h"

namespace engine
{
	class EntityManager
	{
	public:
		using available_entity_container = std::queue<Entity>;
		using used_entity_container = std::set<Entity>;
		using signature_container = std::array<Signature, MAX_ENTITY>;
		using iterator = typename used_entity_container::iterator;

		EntityManager()
		{
			for (Entity entity = 0; entity < MAX_ENTITY; ++entity)
			{
				m_AvailableEntities.push(entity);
			}
		}

		Entity CreateEntity()
		{
			//Too many entities in existence
			ENGINE_ASSERT(m_LivingEntityCount < MAX_ENTITY);

			Entity id = m_AvailableEntities.front();
			m_AvailableEntities.pop();
			m_UsedEntities.insert(id);
			++m_LivingEntityCount;

			return id;
		}

		void DestroyEntity(Entity entity)
		{
			//Entity out of range
			ENGINE_ASSERT(entity < MAX_ENTITY);

			m_Signatures[entity].reset();
			m_AvailableEntities.push(entity);
			m_UsedEntities.erase(entity);
			--m_LivingEntityCount;
		}

		void SetSignature(Entity entity, Signature signature)
		{
			//Entity out of range
			ENGINE_ASSERT(entity < MAX_ENTITY);

			m_Signatures[entity] = signature;
		}

		Signature GetSignature(Entity entity)
		{
			//Entity out of range
			ENGINE_ASSERT(entity < MAX_ENTITY);

			return m_Signatures[entity];
		}

		iterator begin() { return m_UsedEntities.begin(); }
		iterator end() { return m_UsedEntities.end(); }

	private:
		available_entity_container m_AvailableEntities{};
		used_entity_container m_UsedEntities{};
		signature_container m_Signatures{};
		std::uint32_t m_LivingEntityCount{};
	};
}
