#pragma once
#include "ECS_Utility.h"
#include <vector>
namespace engine
{
    class DeletedGameObject
    {
        friend class ECS_Manager;

        Signature signature;
        std::array<Component*,MAX_COMPONENTS> m_componentList;
    public:
        DeletedGameObject();
        ~DeletedGameObject();
    };


}