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
	public:
		using TypeContainer = std::unordered_map<const char*, ComponentType>;
		using ComponentContainer = std::unordered_map<const char*, std::shared_ptr<ComponentArrayBase>>;
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
			++m_NextComponentType;
		}

		template<typename T>
		ComponentType GetComponentID()
		{
			const char* typeName = typeid(T).name();
			//Component not registered before use
			ENGINE_ASSERT(IsRegistered<T>());

			return m_ComponentTypes[typeName];
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
			comp->SetEntity(entity);
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

		/*template<typename T, typename... args>
		std::enable_if_t<std::is_base_of<Component, T>::value, T&> EmplaceComponent(Entity entity,bool active, args&&... arguementList)
		{
			if (IsRegistered<T>() == false)
				RegisterComponent<T>();
			return GetComponentArray<T>()->EmplaceData(entity, entity, active, std::forward<args>(arguementList)...);
		}*/

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
		T& GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->GetData(entity);
		}

		template<typename T>
		T* TryGetComponent(Entity entity)
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
		bool CopyComponent(Entity source, Entity dest)
		{
			if (HasComponent<T>(source) == false)
				return false;
			if (HasComponent<T>(dest))
				GetComponent<T>(dest) = GetComponent<T>(source);
			else
				EmplaceComponent<T>(dest, GetComponent<T>(source));
			return true;
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
		Entity GetEntity(T& component)
		{
			return static_cast<Component>(component).GetEntity();
		}

		template<typename T>
		Entity GetEntity(T* component)
		{
			return static_cast<Component>(component).GetEntity();
		}

		template<typename T>
		typename ComponentArray<T>::container_type& GetContainer()
		{
			return GetComponentArray<T>()->GetContainer();
		}

		template<typename T>
		typename ComponentArray<T>::container_type::dense_container& GetContainerDenseArray()
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


	private:
		TypeContainer m_ComponentTypes{};
		ComponentContainer m_ComponentArrays{};
		ComponentType m_NextComponentType{};


		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			//Component not registered before use
			ENGINE_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end());

			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
		}

		template<typename T>
		bool IsRegistered()
		{
			const char* typeName = typeid(T).name();
			return m_ComponentTypes.find(typeName) != m_ComponentTypes.end();
		}
	};
}
