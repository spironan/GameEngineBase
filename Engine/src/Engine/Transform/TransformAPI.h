#pragma once

#include "Engine/Scripting/ScriptUtilityAPI.h"

#include "TransformSystem.h"
#include "Transform3D.h"
#include "Engine/ECS/GameObject.h"

namespace engine
{
    namespace scriptAPI
    {
        /*********************************************************************************//*!
        \brief      gets the local position of the specified Transform

        \note       Mainly used for C# side calling

        \param      instanceID
                the entity id of the target Transform's GameObject
        \param      x
                the pointer to store the local position x-coordinate to
        \param      y
                the pointer to store the local position y-coordinate to
        \param      z
                the pointer to store the local position z-coordinate to
        *//**********************************************************************************/
        SCRIPT_API void Transform_GetLocalPosition(Entity instanceID, float* x, float* y, float* z);

        /*********************************************************************************//*!
        \brief      sets the local position of the specified Transform

        \note       Mainly used for C# side calling

        \param      instanceID
                the entity id of the target Transform's GameObject
        \param      x
                the local position x-coordinate
        \param      y
                the local position y-coordinate
        \param      z
                the local position z-coordinate
        *//**********************************************************************************/
        SCRIPT_API void Transform_SetLocalPosition(Entity instanceID, float x, float y, float z);

        //SCRIPT_API void Transform_GetGlobalPosition(Entity instanceID, float* x, float* y, float* z);

        //SCRIPT_API void Transform_SetGlobalPosition(Entity instanceID, float x, float y, float z);

        /*********************************************************************************//*!
        \brief      gets the local rotation angle of the specified Transform

        \note       Mainly used for C# side calling

        \param      instanceID
                the entity id of the target Transform's GameObject

        \return     the current angle of rotation of the specified Transform
        *//**********************************************************************************/
        SCRIPT_API float Transform_GetLocalAngle(Entity instanceID);

        /*********************************************************************************//*!
        \brief      sets the local rotation angle of the specified Transform

        \note       Mainly used for C# side calling

        \param      instanceID
                the entity id of the target Transform's GameObject
        \param      angle
                the new angle to use on the specified Transform
        *//**********************************************************************************/
        SCRIPT_API void Transform_SetLocalAngle(Entity instanceID, float angle);

        /*********************************************************************************//*!
        \brief      gets the local scale of the specified Transform

        \note       Mainly used for C# side calling

        \param      instanceID
                the entity id of the target Transform's GameObject
        \param      x
                the pointer to store the local scale x-factor to
        \param      y
                the pointer to store the local scale y-factor to
        \param      z
                the pointer to store the local scale z-factor to
        *//**********************************************************************************/
        SCRIPT_API void Transform_GetLocalScale(Entity instanceID, float* x, float* y, float* z);

        /*********************************************************************************//*!
        \brief      sets the local scale of the specified Transform

        \note       Mainly used for C# side calling

        \param      instanceID
                the entity id of the target Transform's GameObject
        \param      x
                the local scale x-factor
        \param      y
                the local scale y-factor
        \param      z
                the local scale z-factor
        *//**********************************************************************************/
        SCRIPT_API void Transform_SetLocalScale(Entity instanceID, float x, float y, float z);

        /*********************************************************************************//*!
        \brief      gets the number of children the specified Transform has

        \note       Mainly used for C# side calling

        \param      instanceID
                the entity id of the target Transform's GameObject

        \return     the number of children the specified Transform has
        *//**********************************************************************************/
        SCRIPT_API int Transform_GetChildCount(Entity instanceID);

        /*********************************************************************************//*!
        \brief      gets the child of the specified Transform by index

        \note       Mainly used for C# side calling

        \param      instanceID
                the entity id of the target Transform's GameObject
        \param      childIndex
                the child index of the target child

        \return     the IntPtr to the target child of the specified Transform
        *//**********************************************************************************/
        SCRIPT_API uint32_t Transform_GetChild(Entity instanceID, size_t childIndex);

        /*********************************************************************************//*!
        \brief      sets the parent of the specified Transform

        \note       Mainly used for C# side calling

        \param      instanceID
                the entity id of the target Transform's GameObject
        \param      newParent
                the entity id of the new parent Transform's GameObject, or (Entity)-1 if null
        \param      preserveTransforms
                true if the global position of the specified Transform should remain unchanged, else false
        *//**********************************************************************************/
        SCRIPT_API void Transform_SetParent(Entity instanceID, Entity newParent, bool preserveTransforms);

        /*********************************************************************************//*!
        \brief      gets the parent of the specified Transform, if any

        \note       Mainly used for C# side calling

        \param      instanceID
                the entity id of the target Transform's GameObject

        \return     the IntPtr to the Transform C# Interface of the parent Transform, or 0 if there is no parent
        *//**********************************************************************************/
        SCRIPT_API uint32_t Transform_GetParent(Entity instanceID);
    }
}