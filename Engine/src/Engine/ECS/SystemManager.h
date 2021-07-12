/*****************************************************************//**
\file       SystemManager.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui@digipen.edu
\date       2/7/2021
\brief  
Manages all systems that are registered with the system manager.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once

#include <cassert>
#include <memory>
#include <unordered_map>
#include "ECS_Utility.h"
#include "System.h"

namespace engine
{

	class SystemManager
	{
	public:
		template<typename T, typename... Args>
		std::shared_ptr<T> RegisterSystem(Args&&... arguementList)
		{
			const char* typeName = typeid(T).name();

			//Registering system more than once
			ENGINE_ASSERT(m_Systems.find(typeName) == m_Systems.end());

			auto system = std::make_shared<T>(std::forward<Args>(arguementList)...);
			m_Systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();

			assert(m_Systems.find(typeName) != m_Systems.end() && "System used before registered.");

			m_Signatures.insert({ typeName, signature });
		}

		void EntityDestroyed(Entity entity)
		{
			/*for (auto const& pair : m_Systems)
			{
				auto const& system = pair.second;


				system->m_Entities.erase(entity);
			}*/
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			/*for (auto const& pair : m_Systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_Signatures[type];

				if ((entitySignature & systemSignature) == systemSignature)
				{
					system->m_Entities.insert(entity);
				}
				else
				{
					system->m_Entities.erase(entity);
				}
			}*/
		}

	private:
		std::unordered_map<const char*, Signature> m_Signatures{};
		std::unordered_map<const char*, std::shared_ptr<System>> m_Systems{};
	};
}