/*****************************************************************//**
\file       ComponentManager.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui@digipen.edu
\date       29/6/2021
\brief  
Manages the components for each enitity in the ECS system

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once

#include "ComponentArray.h"

namespace engine
{
	class ComponentManager
	{
		friend class ECS_Manager;
	public:
		using TypeContainer = std::unordered_map<const char*, ComponentType>;
		using ComponentContainer = std::unordered_map<const char*, std::shared_ptr<ComponentArrayBase>>;
		using AddComponentCallback = std::function<void*(ComponentManager&, Entity, void*)>;
		using AddComponentMap = std::unordered_map<ComponentType, AddComponentCallback>;
		using GetComponentCallback = std::function<void* (ComponentManager&, Entity)>;
		using GetComponentMap = std::unordered_map<ComponentType, GetComponentCallback>;
		using CopyComponentCallback = std::function<bool (ComponentManager&, Entity, Entity)>;
		using CopyComponentMap = std::unordered_map<ComponentType, CopyComponentCallback>;
		using DeletedComponentCallback = std::function<void*(ComponentManager&, Entity)>;
		using DeletedComponentMap = std::unordered_map<ComponentType, DeletedComponentCallback>;
		using RestoredComponentCallback = std::function<void* (ComponentManager&, Entity, void*)>;
		using RestoredComponentMap = std::unordered_map<ComponentType, RestoredComponentCallback>;

		/*****************************************************************//**
		 * @brief Registers a component to be used. Creates a ComponentArray
		 * of type T and inserts it into m_ComponentArrays
		 * 
		*********************************************************************/
		template<typename T>
		void RegisterComponent() 
		{
			const char* typeName = typeid(T).name();

			//Registering component type more than once
			if (IsRegistered<T>()) return;

			m_ComponentTypes.insert({ typeName, m_NextComponentType });
			m_ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
			m_addComponentMap.insert({ m_NextComponentType, [](ComponentManager& cm,Entity entity, void* comp)->void* {
				//return reinterpret_cast<Component*>(&(cm.EmplaceComponent<T>(entity)));
				return &(cm.AddComponent(entity,*(static_cast<T*>(comp))));
				} });
			m_getComponentMap.insert({ m_NextComponentType, [](ComponentManager& cm,Entity entity)->void* {
				return &(cm.GetComponent<T>(entity));
				} });
			m_copyComponentMap.insert({ m_NextComponentType, [](ComponentManager& cm,Entity src,Entity dest)->bool {
				return cm.CopyComponent<T>(src,dest);
				} });
			m_deletedComponentMap.insert({ m_NextComponentType, [](ComponentManager& cm,Entity entity)->void* {
				T* deleted_component = reinterpret_cast<T*>(new char[sizeof(T)]);
				memcpy(deleted_component, &(cm.GetComponent<T>(entity)), sizeof(T));
				//LOG_TRACE("store deleted component:{0}, from {1}", typeid(T).name(), entity);
				return static_cast<void*>(deleted_component);
				} });
			m_restoredComponentMap.insert({ m_NextComponentType, [](ComponentManager& cm,Entity entity, void* component)->void* {
				//LOG_TRACE("added restored component:{0}, to {1}", typeid(T).name(), entity);
				T& restored_component = cm.EmplaceComponent<T>(entity, *(static_cast<T*>(component)));
				if constexpr (std::is_base_of<Component, T>::value == true)
					restored_component.SetEntity(entity);
				return static_cast<void*>(&restored_component);
				} });
			++m_NextComponentType;
		}

		template<typename T>
		void RegisterComponent() const
		{
			RegisterComponent<T>();
		}

		template<typename T>
		ComponentType GetComponentID() 
		{
			const char* typeName = typeid(T).name();
			//Component not registered before use
			if (IsRegistered<T>() == false)
				RegisterComponent<T>();

			return m_ComponentTypes.find(typeName)->second;
		}

		/*template<typename T>
		T& AddComponent(Entity entity, T component)
		{
			if (IsRegistered<T>() == false)
				RegisterComponent<T>();
			return *GetComponentArray<T>()->InsertData(entity, component);
		}*/

		template<typename T>
		std::enable_if_t<std::is_base_of<Component,T>::value,T&> AddComponent(Entity entity, T component)
		{
			if (IsRegistered<T>() == false)
				RegisterComponent<T>();
			auto& comp = GetComponentArray<T>()->InsertData(entity, component);			
			comp.SetEntity(entity);
			return comp;
		}

		template<typename T>
		std::enable_if_t<std::is_base_of<Component, T>::value == false, T&> AddComponent(Entity entity, T component)
		{
			if (IsRegistered<T>() == false)
				RegisterComponent<T>();
			auto& comp = GetComponentArray<T>()->InsertData(entity, component);
			return comp;
		}

		template<typename T, typename... args>
		std::enable_if_t<std::is_base_of<Component, T>::value == false, T&> EmplaceComponent(Entity entity, args&&... arguementList)			
		{
			if (IsRegistered<T>() == false)
				RegisterComponent<T>();
			return GetComponentArray<T>()->EmplaceData(entity, std::forward<args>(arguementList)...);
		}

		template<typename T, typename... args>
		std::enable_if_t<std::is_base_of<Component, T>::value, T&> EmplaceComponent(Entity entity, args&&... arguementList)
		{
			if (IsRegistered<T>() == false)
				RegisterComponent<T>();
			return GetComponentArray<T>()->EmplaceData(entity, std::forward<args>(arguementList)...);
		}

		void* EmplaceComponentByTypeID(Entity entity, ComponentType type, void* component)
		{
			if (type >= Size())
				return nullptr;
			return m_addComponentMap[type](*this, entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		bool HasComponent(Entity entity)
		{
			if (!IsRegistered<T>()) { return false; }
			return GetComponentArray<T>()->HasData(entity);
		}

		template<typename T>
		bool HasComponent(Entity entity) const
		{
			ENGINE_ASSERT(IsRegistered<T>());
			return GetComponentArray<T>()->HasData(entity);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->GetData(entity);
		}

		template<typename T>
		T const& GetComponent(Entity entity) const
		{
			return GetComponent<T>();
		}

		void* GetDeletedComponentByTypeID(Entity entity, ComponentType type) 
		{
			if (type >= Size())
				return nullptr;
			return m_deletedComponentMap[type](*this, entity);
		}

		void* RestoreComponentByTypeID(Entity entity, ComponentType type,void* component)
		{
			if (type >= Size())
				return nullptr;
			return m_restoredComponentMap[type](*this, entity, component);
		}
		
		void* GetComponentByTypeID(Entity entity, ComponentType type)
		{
			if (type >= Size())
				return nullptr;
			return m_getComponentMap[type](*this,type);
		}

		template<typename... Component>
		decltype(auto) GetComponents(Entity entity)
		{
			if constexpr (sizeof...(Component) == 1) 
			{
				return GetComponentArray<Component>()->GetData(entity);
			}

			return std::forward_as_tuple(GetComponentArray<Component>()->GetData(entity)...);
		}

		template<typename... Component>
		decltype(auto) GetComponents(Entity entity) const
		{
			if constexpr (sizeof...(Component) == 1)
			{
				return GetComponentArray<Component>()->GetData(entity);
			}

			return std::forward_as_tuple(GetComponentArray<Component>()->GetData(entity)...);
		}

		template<typename T>
		T* TryGetComponent(Entity entity)
		{
			return GetComponentArray<T>()->TryGetData(entity);
		}

		template<typename T>
		T const* TryGetComponent(Entity entity) const
		{
			return GetComponentArray<T>()->TryGetData(entity);
		}

		template<typename T>
		bool TransferComponent(Entity source, Entity dest)
		{
			if (HasComponent<T>(source) == false || HasComponent<T>(dest))
				return false;
			EmplaceComponent<T>(dest, GetComponent<T>(source));
			RemoveComponent<T>(source);
			return true;
		}

		template<typename T>
		std::enable_if_t<std::is_base_of<Component, T>::value == false, bool> CopyComponent(Entity source, Entity dest)
		{
			if (HasComponent<T>(source) == false || HasComponent<T>(dest) == false)
				return false;
			GetComponent<T>(dest) = GetComponent<T>(source);
			return true;
		}
		
		template<typename T>
		std::enable_if_t<std::is_base_of<Component, T>::value == true, bool> CopyComponent(Entity source, Entity dest)
		{
			if (HasComponent<T>(source) == false || HasComponent<T>(dest) == false)
				return false;

			GetComponent<T>(dest).CopyComponent(GetComponent<T>(source));
			GetComponent<T>(dest).SetEntity(dest);
			return true;
		}

		
		bool CopyComponentByTypeID(Entity source, Entity dest, ComponentType type)
		{
			if (type >= Size())
				return false;
			return m_copyComponentMap[type](*this, source, dest);
		}

		void OnEntityDestroy(Entity entity)
		{
			for (auto const& pair : m_ComponentArrays)
			{
				auto const& component = pair.second;

				component->OnEntityDestroy(entity);
			}
		}

		template<typename T>
		Entity GetEntity(T& component) const
		{
			return static_cast<Component>(component).GetEntity();
		}

		template<typename T>
		Entity GetEntity(T* component) const
		{
			return static_cast<Component>(component).GetEntity();
		}

		template<typename T>
		typename ComponentArray<T>::container_type& GetContainer()
		{
			return GetComponentArray<T>()->GetContainer();
		}

		template<typename T>
		typename ComponentArray<T>::container_type const& GetContainer() const
		{
			return GetComponentArray<T>()->GetContainer();
		}

		template<typename T>
		typename ComponentArray<T>::container_type::dense_container& GetContainerDenseArray()
		{
			return GetContainer<T>().GetDenseContainer();
		}

		template<typename T>
		typename ComponentArray<T>::container_type::dense_container const& GetContainerDenseArray() const
		{
			return GetContainer<T>().GetDenseContainer();
		}

		template<typename T>
		std::enable_if_t<std::is_base_of<Component, T>::value, void> Swap(typename ComponentArray<T>::container_type::dense_container::size_type index1,
			typename ComponentArray<T>::container_type::dense_container::size_type index2)
		{
			auto& cont = GetContainer();
			cont.Swap(index1, index2);
			cont.AtIndex(index1).SetEntity(cont.AtIndexSparse(index1));
			cont.AtIndex(index2).SetEntity(cont.AtIndexSparse(index2));
		}

		template<typename T>
		std::enable_if_t<std::is_base_of<Component, T>::value == false, void> Swap(typename ComponentArray<T>::container_type::dense_container::size_type index1,
			typename ComponentArray<T>::container_type::dense_container::size_type index2)
		{
			GetContainer().Swap(index1, index2);
		}

		template<typename T>
		bool IsRegistered() const
		{
			const char* typeName = typeid(T).name();
			return m_ComponentTypes.find(typeName) != m_ComponentTypes.end();
		}

		ComponentType Size() const
		{
			return m_NextComponentType;
		}
	private:
		TypeContainer m_ComponentTypes{};
		ComponentContainer m_ComponentArrays{};
		ComponentType m_NextComponentType{};
		AddComponentMap m_addComponentMap{};
		GetComponentMap m_getComponentMap{};
		CopyComponentMap m_copyComponentMap{};
		DeletedComponentMap m_deletedComponentMap{};
		RestoredComponentMap m_restoredComponentMap{};

		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			//Component not registered before use
			if (IsRegistered<T>() == false)
				RegisterComponent<T>();


			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
		}

		template<typename T>
		std::weak_ptr<ComponentArray<T> const> GetComponentArray() const
		{
			return const_cast<ComponentManager&>(*this).GetComponentArray<T>();
		}
	};
}
