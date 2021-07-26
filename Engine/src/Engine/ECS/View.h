/*****************************************************************//**
\file       View.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui@digipen.edu
\date       2/7/2021
\brief  
View iterates over those entities that have at least all the
given components 
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once

#include "ECS_Utility.h"
#include "ComponentManager.h"
#include "EntityManager.h"
namespace engine
{

	template<typename... Args>
	struct ComponentView
	{
		friend class ECS_Manager;
	public:
		template<typename T>
		class Iterator
		{
		public:
			using difference_type = typename std::iterator_traits<T>::difference_type;
			using value_type = typename std::iterator_traits<T>::value_type;
			using pointer = typename std::iterator_traits<T>::pointer;
			using reference = typename std::iterator_traits<T>::reference;

			Signature m_signature;
			T m_first, m_last, m_curr;
			EntityManager& m_entityManager;

			Iterator(EntityManager& entityManager, Signature signature, T first, T last, T curr) :
				m_entityManager(entityManager),
				m_signature(signature),
				m_first(first),
				m_last(last),
				m_curr(curr)
			{
				if (curr != last && IsValid() == false)
					++(*this);
			}


			bool IsValid() const 
			{
				return (m_entityManager.GetSignature(*m_curr) & m_signature) == m_signature;
			}


			//pre
			Iterator& operator++() 
			{
				while (++m_curr != m_last && IsValid() == false);
				return *this;
			};
			Iterator& operator--() 
			{
				while (--m_curr != m_first && IsValid() == false);
				return *this;
			};

			//post
			Iterator operator++(int) 
			{
				Iterator original = *this;
				++(*this);
				return original;
			};
			Iterator operator--(int) 
			{
				Iterator original = *this;
				--(*this);
				return original;
			};

			bool operator==(const Iterator& other) const
			{
				return other.m_curr == m_curr;
			}
				
			bool operator!=(const Iterator& other) const
			{
				return !(*this == other);
			}

			pointer operator->() const
			{
				return &*m_curr;
			}

			reference operator*() const 
			{
				return *operator->();
			}
		};
	public:
		using iterator = typename EntityManager::iterator;
		using view_iterator = Iterator<iterator>;
	private:
		Signature m_signature;
		iterator m_begin, m_end;
		EntityManager& m_entityManager;

		template<typename Aux_type>
		void SetSignature(ComponentManager& componentManager){}

		template<typename Aux_type, typename Type, typename... Args>
		void SetSignature(ComponentManager& componentManager)
		{
			m_signature.set(componentManager.GetComponentID<Type>(), true);
			//LOG_ENGINE_TRACE("View set signature");
			//LOG_ENGINE_TRACE(componentManager.GetComponentID<Type>());
			SetSignature<Aux_type, Args...>(componentManager);
		}

	public:

		explicit ComponentView(ComponentManager& componentManager, EntityManager& entityManager) : 
			m_begin(entityManager.begin()),
			m_end(entityManager.end()),
			m_entityManager(entityManager)
		{
			SetSignature<void, Args...>(componentManager);
		}

		ComponentView(ComponentView const&) = default;
		~ComponentView() = default;

		view_iterator begin() { return view_iterator(m_entityManager, m_signature, m_begin, m_end, m_begin); }
		view_iterator end() { return view_iterator(m_entityManager, m_signature, m_begin, m_end, m_end); }


		//operator bool() {};
		////pre
		//ComponentView& operator++() {};
		//ComponentView& operator--() {};

		////post
		//ComponentView& operator++(int) {};
		//ComponentView& operator--(int) {};

		//bool operator==(ComponentView const& other) const {};
		//bool operator!=(ComponentView const& other) const {};

		//Entity operator->()
	};


}