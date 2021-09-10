#pragma once

#include "Engine/Scripting/ScriptUtilityAPI.h"

#include "TransformSystem.h"
#include "Transform3D.h"
#include "Engine/ECS/GameObject.h"

namespace engine
{
    namespace scriptAPI
    {
        SCRIPT_API void Transform_GetLocalPosition(Entity instanceID, float* x, float* y, float* z);

        SCRIPT_API void Transform_SetLocalPosition(Entity instanceID, float x, float y, float z);

        //SCRIPT_API void Transform_GetGlobalPosition(Entity instanceID, float* x, float* y, float* z);

        //SCRIPT_API void Transform_SetGlobalPosition(Entity instanceID, float x, float y, float z);



        SCRIPT_API float Transform_GetLocalAngle(Entity instanceID);

        SCRIPT_API void Transform_SetLocalAngle(Entity instanceID, float angle);



        SCRIPT_API void Transform_GetLocalScale(Entity instanceID, float* x, float* y, float* z);

        SCRIPT_API void Transform_SetLocalScale(Entity instanceID, float x, float y, float z);



        SCRIPT_API int Transform_GetChildCount(Entity instanceID);

        SCRIPT_API uint32_t Transform_GetChild(Entity instanceID, size_t childIndex);

        SCRIPT_API void Transform_SetParent(Entity instanceID, Entity newParent, bool preserveTransforms);

        SCRIPT_API uint32_t Transform_GetParent(Entity instanceID);
    }
}