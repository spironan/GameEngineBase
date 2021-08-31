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
#include "DeletedGameObject.h"
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
			deleteList.emplace(entity);
			

			//m_SystemManager->EntityDestroyed(entity);
		}

		void ProcessDeletions()
		{
			for (auto const i : deleteList)
			{
				m_EntityManager->DestroyEntity(i);
				m_ComponentManager->OnEntityDestroy(i);
			}
			deleteList.clear();
		}


		// Component methods
		template<typename T>
		void RegisterComponent()
		{
			m_ComponentManager->RegisterComponent<T>();
		}

		template<typename T>
		T& AddComponent(Entity entity, T component)
		{
			auto comp = m_ComponentManager->AddComponent<T>(entity, component);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentID<T>(), true);
			m_EntityManager->SetSignature(entity, signature);

			//m_SystemManager->EntitySignatureChanged(entity, signature);
			return comp;
		}

		template<typename T, typename... args>
		std::enable_if_t<std::is_base_of_v<Component, T> == true, T&>
			EmplaceComponent(Entity entity, args&&... arguementList)
		{
			auto& comp = m_ComponentManager->EmplaceComponent<T>(entity, entity, std::forward<args>( arguementList)...);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentID<T>(), true);
			m_EntityManager->SetSignature(entity, signature);

			//m_SystemManager->EntitySignatureChanged(entity, signature);
			return comp;
		}

		template<typename T, typename... args>
		std::enable_if_t<std::is_base_of_v<Component, T> == false, T&> 
			EmplaceComponent(Entity entity, args&&... arguementList)
		{
			auto& comp = m_ComponentManager->EmplaceComponent<T>(entity, std::forward<args>(arguementList)...);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentID<T>(), true);
			m_EntityManager->SetSignature(entity, signature);

			//m_SystemManager->EntitySignatureChanged(entity, signature);
			return comp;
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			ENGINE_ASSERT(m_EntityManager->Valid(entity));
			m_ComponentManager->RemoveComponent<T>(entity);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentID<T>(), false);
			m_EntityManager->SetSignature(entity, signature);

			//m_SystemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			ENGINE_ASSERT(m_EntityManager->Valid(entity));
			return m_ComponentManager->GetComponent<T>(entity);
		}

		template<typename T>
		T const& GetComponent(Entity entity) const
		{
			ENGINE_ASSERT(m_EntityManager->Valid(entity));
			return m_ComponentManager->GetComponent<T>(entity);
		}
		
		template<typename... Component>
		decltype(auto) GetComponents(Entity entity)
		{
			ENGINE_ASSERT(m_EntityManager->Valid(entity));
			return m_ComponentManager->GetComponents<Component...>(entity);
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
		bool HasComponent(Entity entity) const
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
			return m_SystemManager->RegisterSystem<T>(*this, std::forward<Args>(arguementList)...);
		}

		template<typename T>
		T* GetSystem()
		{
			return m_SystemManager->GetSystem<T>();
		}

		template<typename T>
		T* TryGetSystem()
		{
			return m_SystemManager->TryGetSystem<T>();
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

		template<typename T>
		typename ComponentArray<T>::container_type& GetComponentContainer()
		{
			return m_ComponentManager->GetContainer<T>();
		}

		template<typename T>
		typename ComponentArray<T>::container_type::dense_container& GetComponentDenseArray()
		{
			return m_ComponentManager->GetContainerDenseArray<T>();
		}

		template<typename T>
		void Swap(typename ComponentArray<T>::container_type::dense_container::size_type index1,
			typename ComponentArray<T>::container_type::dense_container::size_type index2)
		{
			return m_ComponentManager->Swap<T>(index1, index2);
		}

		Entity DuplicateEntity(Entity source)
		{
			Entity dest = CreateEntity();
			return DuplicateEntity(source, dest);
		}

		Entity DuplicateEntity(Entity source, Entity dest)
		{
			auto signature = m_EntityManager->GetSignature(dest);
			for (ComponentType type = 0; type < m_ComponentManager->Size(); ++type)
			{
				if (signature[type] == true)
				{
					bool result = m_ComponentManager->CopyComponentByTypeID(source, dest, type);
					ENGINE_ASSERT(result);
				}
			}
			return dest;
		}

		auto StoreAsDeleted(Entity entity)
		{
			std::shared_ptr<DeletedGameObject> temp = std::make_shared<DeletedGameObject>();
			auto signature = m_EntityManager->GetSignature(entity);
			temp->signature = signature;
			for (ComponentType type = 0; type < m_ComponentManager->Size(); ++type)
			{
				if (signature[type] == true)
				{
					void* component = m_ComponentManager->GetDeletedComponentByTypeID(entity, type);
					ENGINE_ASSERT(component);
					temp->m_componentList[type] = static_cast<Component*>(component);
				}
			}

			return temp;
		}

		Entity RestoreFromDeleted(DeletedGameObject& go)
		{
			Entity entity = CreateEntity();
			auto signature = go.signature;
			for (ComponentType type = 0; type < m_ComponentManager->Size(); ++type)
			{
				if (signature[type] == true)
				{
					void* component = static_cast<void*>(go.m_componentList[type]);
					auto restored_component = m_ComponentManager->RestoreComponentByTypeID(entity, type, component);
					ENGINE_ASSERT(restored_component);
				}
			}
			return entity;
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

	protected:
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<EntityManager> m_EntityManager;
		//std::unique_ptr<EventManager> mEventManager;
		std::unique_ptr<SystemManager> m_SystemManager;
		std::set<Entity> deleteList;
	};
}
