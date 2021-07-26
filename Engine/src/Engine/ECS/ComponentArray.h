/*****************************************************************//**
\file   ComponentArray.h
\author Lim Guan Hui, l.guanhui , 2000552
\email  l.guanhui@digipen.edu
\date   29/6/2021
\brief  
Container for the array of components. Internally uses a sparse set
data structure for managing its elements

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "ECS_Utility.h"
#include "SparseArray.h"
#include "Component.h"
#include "Engine/Core/Assert.h"

namespace engine
{

    class ComponentArrayBase
    {
    public:
        virtual ~ComponentArrayBase() = default;
        virtual void OnEntityDestroy(Entity entity) = 0;
    private:

    };

    template<typename T, typename Allocator = std::allocator<T>>
    class ComponentArray : public ComponentArrayBase
    {
    public:
        using size_type = std::size_t;
        using container_type = SparseContainer<T, MAX_ENTITY>;

        T& InsertData(Entity entity, T component)
        {
            return *m_sparseContainer.insert(entity, component);
        }

        template <typename... args>
        T& EmplaceData(Entity entity, args&&... arguementList)
        {
            return *m_sparseContainer.emplace(entity, std::forward<args>(arguementList)...);
        }

        void RemoveData(Entity entity)
        {
            m_sparseContainer.Remove(entity);
        }
        T& GetData(Entity entity)
        {
            ENGINE_ASSERT(m_sparseContainer[entity]);
            return *m_sparseContainer[entity];
        }

        T* TryGetData(Entity entity)
        {
            return m_sparseContainer[entity];
        }

        void OnEntityDestroy(Entity entity) override
        {
            RemoveData(entity);
        }

        bool HasData(Entity entity)
        {
            return !m_sparseContainer.IsAvailable(entity);
        }

        container_type& GetContainer() { return m_sparseContainer; }
    private:
        container_type m_sparseContainer;
    };

}
