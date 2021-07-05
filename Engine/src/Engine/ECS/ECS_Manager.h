/*****************************************************************//**
\file       ECS_Manager.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui@digipen.edu
\date       29/6/2021
\brief  
An overall manager that provides an interface for the ECS system
to be used. 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "ECS_Utility.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "View.h"
namespace engine
{
	class ECS_Manager
	{
	public:

		void Init()
		{
			m_ComponentManager = std::make_unique<ComponentManager>();
			m_EntityManager = std::make_unique<EntityManager>();
			//mEventManager = std::make_unique<EventManager>();
			m_SystemManager = std::make_unique<SystemManager>();
		}


		// Entity methods
		Entity CreateEntity()
		{
			return m_EntityManager->CreateEntity();
		}

		void DestroyEntity(Entity entity)
		{
			m_EntityManager->DestroyEntity(entity);
			m_ComponentManager->OnEntityDestroy(entity);

			//m_SystemManager->EntityDestroyed(entity);
		}


		// Component methods
		template<typename T>
		void RegisterComponent()
		{
			m_ComponentManager->RegisterComponent<T>();
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			m_ComponentManager->AddComponent<T>(entity, component);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentID<T>(), true);
			m_EntityManager->SetSignature(entity, signature);

			//m_SystemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T, typename... args>
		void EmplaceComponent(Entity entity, args&&... arguementList)
		{
			m_ComponentManager->EmplaceComponent<T>(entity, std::forward<args>(arguementList)...);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentID<T>(), true);
			m_EntityManager->SetSignature(entity, signature);

			//m_SystemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			m_ComponentManager->RemoveComponent<T>(entity);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentID<T>(), false);
			m_EntityManager->SetSignature(entity, signature);

			//m_SystemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return m_ComponentManager->GetComponent<T>(entity);
		}

		template<typename T>
		T* TryGetComponent(Entity entity)
		{
			return m_ComponentManager->TryGetComponent<T>(entity);
		}

		template<typename T>
		bool HasComponent(Entity entity)
		{
			return m_ComponentManager->HasComponent<T>(entity);
		}

		template<typename T>
		bool TransferComponent(Entity source, Entity dest)
		{
			return m_ComponentManager->TransferComponent<T>(source, dest);
		}

		template<typename T>
		bool CopyComponent(Entity source, Entity dest)
		{
			return m_ComponentManager->CopyComponent<T>(source, dest);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return m_ComponentManager->GetComponentID<T>();
		}


		// System methods
		template<typename T, typename... Args>
		std::shared_ptr<T> RegisterSystem(Args&&... arguementList)
		{
			return m_SystemManager->RegisterSystem<T>(std::forward<Args>(arguementList)...);
		}

		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			m_SystemManager->SetSignature<T>(signature);
		}

		template<typename... Args>
		ComponentView<Args...> GetComponentView()
		{
			return ComponentView<Args...>(*m_ComponentManager, *m_EntityManager);
		}

		//// Event methods
		//void AddEventListener(EventId eventId, std::function<void(Event&)> const& listener)
		//{
		//	mEventManager->AddListener(eventId, listener);
		//}

		//void SendEvent(Event& event)
		//{
		//	mEventManager->SendEvent(event);
		//}

		//void SendEvent(EventId eventId)
		//{
		//	mEventManager->SendEvent(eventId);
		//}

	private:
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<EntityManager> m_EntityManager;
		//std::unique_ptr<EventManager> mEventManager;
		std::unique_ptr<SystemManager> m_SystemManager;
	};
}
