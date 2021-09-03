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
		using DeletedObject = DeletedGameObject;
		using DeletedObjectPtr = std::shared_ptr<DeletedGameObject>;
		/*********************************************************************************//*!
		\brief    To be called once upon creation of ECS_Manager for initialization
		 
		
		*//**********************************************************************************/
		void Init()
		{
			m_ComponentManager = std::make_unique<ComponentManager>();
			m_EntityManager = std::make_unique<EntityManager>();
			//mEventManager = std::make_unique<EventManager>();
			m_SystemManager = std::make_unique<SystemManager>();
		}


		/*********************************************************************************//*!
		\brief    Create an entity 
		 
		\return   a newly created entity
		
		*//**********************************************************************************/
		Entity CreateEntity()
		{
			return m_EntityManager->CreateEntity();
		}
		/*********************************************************************************//*!
		\brief    Registers an entity to be deleted. Call ProcessDeletions() to carry out the
		destruction of entities
		 
		\param    entity to be deleted
		
		*//**********************************************************************************/
		void DestroyEntity(Entity entity)
		{
			deleteList.emplace(entity);
			

			//m_SystemManager->EntityDestroyed(entity);
		}
		/*********************************************************************************//*!
		\brief    Destroys all entities registered for deletion as well as their attached
		components
		 
		
		*//**********************************************************************************/
		void ProcessDeletions()
		{
			for (auto const i : deleteList)
			{
				m_EntityManager->DestroyEntity(i);
				m_ComponentManager->OnEntityDestroy(i);
			}
			deleteList.clear();
		}


		/*********************************************************************************//*!
		\brief    Registers a component
		 
		
		*//**********************************************************************************/
		template<typename T>
		void RegisterComponent()
		{
			m_ComponentManager->RegisterComponent<T>();
		}
		/*********************************************************************************//*!
		\brief    Adds a component to an entity by copy
		 
		\param    entity 
		entity to add this component to
		\param    
		component component to add
		\return   
		reference to added component
		
		*//**********************************************************************************/
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
		/*********************************************************************************//*!
		\brief    Adds a component to an entity by constructing it in place using the given 
		arguments which are forwarded to the component's constructor. 
		 
		\param    entity
		entity to add this component to
		\param    ...arguementList
		arguments to be forwarded to the component's constructor
		\return
		reference to added component
		
		*//**********************************************************************************/
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
		/*********************************************************************************//*!
		\brief    Adds a component to an entity by constructing it in place using the given
		arguments which are forwarded to the component's constructor.

		\param    entity
		entity to add this component to
		\param    ...arguementList
		arguments to be forwarded to the component's constructor
		\return
		reference to added component

		*//**********************************************************************************/
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
		/*********************************************************************************//*!
		\brief    Removes a component from an entity
		 
		\param    entity
		entity to remove this component from
		
		*//**********************************************************************************/
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
		/*********************************************************************************//*!
		\brief    Gets a component attached to an entity by reference
		 
		\param    entity
		entity to get component from
		\return   
		reference to component
		*//**********************************************************************************/
		template<typename T>
		T& GetComponent(Entity entity)
		{
			ENGINE_ASSERT(m_EntityManager->Valid(entity));
			return m_ComponentManager->GetComponent<T>(entity);
		}
		/*********************************************************************************//*!
		\brief    GetComponent const version 
		 
		\param    entity
		entity to get component from
		\return
		const reference to component		
		*//**********************************************************************************/
		template<typename T>
		T const& GetComponent(Entity entity) const
		{
			ENGINE_ASSERT(m_EntityManager->Valid(entity));
			return m_ComponentManager->GetComponent<T>(entity);
		}
		/*********************************************************************************//*!
		\brief    Gets multiple components attached to an entity by reference in a tuple
		 
		\param    entity
		entity to get components from
		\return   
		tuple containing references to components
		*//**********************************************************************************/
		template<typename... Component>
		decltype(auto) GetComponents(Entity entity)
		{
			ENGINE_ASSERT(m_EntityManager->Valid(entity));
			return m_ComponentManager->GetComponents<Component...>(entity);
		}
		/*********************************************************************************//*!
		\brief    Same as GetComponent but returns a pointer instead and also returns nullptr
		if component does not exist
		 
		\param    entity
		entity to get component from
		\return   
		pointer to component or nullptr if component does not exist
		*//**********************************************************************************/
		template<typename T>
		T* TryGetComponent(Entity entity)
		{
			return m_ComponentManager->TryGetComponent<T>(entity);
		}
		/*********************************************************************************//*!
		\brief    Checks if an entity has a component
		 
		\param    entity
		entity to check
		\return   
		true if entity has the component, false otherwise
		*//**********************************************************************************/
		template<typename T>
		bool HasComponent(Entity entity)
		{
			return m_ComponentManager->HasComponent<T>(entity);
		}
		/*********************************************************************************//*!
		\brief    HasComponent const version
		 
		\param    entity
		entity to check
		\return
		true if entity has the component, false otherwise		
		*//**********************************************************************************/
		template<typename T>
		bool HasComponent(Entity entity) const
		{
			return m_ComponentManager->HasComponent<T>(entity);
		}
		/*********************************************************************************//*!
		\brief    Transfers a component from one entity(source) to another(dest). 
		Source entity will no longer have the component.
		 
		\param    source
		entity to transfer the component from
		\param    dest
		entity to transfer the component to
		\return   
		true if transfer succeeded, false otherwise
		*//**********************************************************************************/
		template<typename T>
		bool TransferComponent(Entity source, Entity dest)
		{
			return m_ComponentManager->TransferComponent<T>(source, dest);
		}
		/*********************************************************************************//*!
		\brief    Copies a component from one entity(source) to another(dest).
		\param    source
		entity to copy the component from
		\param    dest
		entity to copy the component to
		\return
		true if copy succeeded, false otherwise		
		*//**********************************************************************************/
		template<typename T>
		bool CopyComponent(Entity source, Entity dest)
		{
			return m_ComponentManager->CopyComponent<T>(source, dest);
		}
		/*********************************************************************************//*!
		\brief    Get the id of a component.
		 
		\return   
		id of a component
		*//**********************************************************************************/
		template<typename T>
		ComponentType GetComponentType()
		{
			return m_ComponentManager->GetComponentID<T>();
		}


		/*********************************************************************************//*!
		\brief    Registers a system to be used.
		 
		\param    ...arguementList
		Arguments to be forwarded to constructor of the system
		\return   
		shared pointer to the registered system
		*//**********************************************************************************/
		template<typename T, typename... Args>
		std::shared_ptr<T> RegisterSystem(Args&&... arguementList)
		{
			return m_SystemManager->RegisterSystem<T>(*this, std::forward<Args>(arguementList)...);
		}
		/*********************************************************************************//*!
		\brief    Gets a pointer to a registered system
		 
		\return   
		pointer to a registered system
		*//**********************************************************************************/
		template<typename T>
		T* GetSystem()
		{
			return m_SystemManager->GetSystem<T>();
		}
		/*********************************************************************************//*!
		\brief    Same as GetSystem but returns null if system has not been registered
		 
		\return
		pointer to a registered system or null if system has not been registered		
		*//**********************************************************************************/
		template<typename T>
		T* TryGetSystem()
		{
			return m_SystemManager->TryGetSystem<T>();
		}
		/*********************************************************************************//*!
		\brief    Sets a system's signature
		 
		\param    signature
		system's signature
		*//**********************************************************************************/
		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			m_SystemManager->SetSignature<T>(signature);
		}
		/*********************************************************************************//*!
		\brief    Gets a view object used to iterate through all entities containing certain
		components
		 
		\return   
		view object
		*//**********************************************************************************/
		template<typename... Args>
		ComponentView<Args...> GetComponentView()
		{
			return ComponentView<Args...>(*m_ComponentManager, *m_EntityManager);
		}
		/*********************************************************************************//*!
		\brief    Get the sparse array container of a registered component
		 
		\return   
		sparse array container of a registered component
		*//**********************************************************************************/
		template<typename T>
		typename ComponentArray<T>::container_type& GetComponentContainer()
		{
			return m_ComponentManager->GetContainer<T>();
		}
		/*********************************************************************************//*!
		\brief    Get the sparse array container's dense array of a registered component
		 
		\return   
		sparse array container's dense array of a registered component
		*//**********************************************************************************/
		template<typename T>
		typename ComponentArray<T>::container_type::dense_container& GetComponentDenseArray()
		{
			return m_ComponentManager->GetContainerDenseArray<T>();
		}
		/*********************************************************************************//*!
		\brief    Swaps two elements in a sparse array container's dense array of a 
		registered component by index
		 
		\param    index1
		1st element to be swapped
		\param    index2
		2nd element to be swapped		
		*//**********************************************************************************/
		template<typename T>
		void Swap(typename ComponentArray<T>::container_type::dense_container::size_type index1,
			typename ComponentArray<T>::container_type::dense_container::size_type index2)
		{
			return m_ComponentManager->Swap<T>(index1, index2);
		}
		/*********************************************************************************//*!
		\brief    Copies an entity's(source) components and attaches it to a newly created 
		entity
		 
		\param    source
		source entity
		\return   
		the newly created entity with the copied components
		*//**********************************************************************************/
		Entity DuplicateEntity(Entity source)
		{
			Entity dest = CreateEntity();
			return DuplicateEntity(source, dest);
		}
		/*********************************************************************************//*!
		\brief    Copies an entity's(source) components and attaches it to a given entity
		 
		\param    source
		source entity
		\param    dest
		entity to receive copy of source's components
		\return   
		dest
		*//**********************************************************************************/
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
		/*********************************************************************************//*!
		\brief    Stores an entity as a special deleted object, which allows it to be restored
		later using the function RestoreFromDeleted
		 
		\param    entity
		entity to store the data as special deleted object
		\return   
		shared pointer to a special deleted object
		*//**********************************************************************************/
		auto StoreAsDeleted(Entity entity)
		{
			DeletedObjectPtr temp = std::make_shared<DeletedObject>();
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
		/*********************************************************************************//*!
		\brief    Restores a entity from data in a special deleted object created from the
		function StoreAsDeleted
		 
		\param    go
		reference to the special deleted object
		\return   
		restored entity
		*//**********************************************************************************/
		Entity RestoreFromDeleted(DeletedObject& go)
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
			m_EntityManager->SetSignature(entity, signature);
			return entity;
		}

		void Clear()
		{
			for (auto i : m_EntityManager->m_UsedEntities)
			{
				DestroyEntity(i);
			}
			ProcessDeletions();
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
