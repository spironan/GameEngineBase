/************************************************************************************//*!
\file           TransformSystem.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 07, 2021
\brief          Defines the Transform System which is responsible for updating
                all transform components in the Entity Component System.
 
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/ECS/System.h"

//#include "SceneGraphList.h"

namespace engine 
{
    //forward declaration
    class GameObject;
    class Transform3D;

    /********************************************************************************//*!
     @brief    Defines the Transform Runtime which is responsible for updating
               all transform components in the system.
    ************************************************************************************/
    class TransformSystem final : public System
    {
      public:
        /*-----------------------------------------------------------------------------*/
        /* Class Specific Type Aliases                                                 */
        /*-----------------------------------------------------------------------------*/
        using value_type                = Transform3D;
        using value_reference           = value_type&;
        using value_const_reference     = value_type const&;
        
        using container_type            = std::vector<value_type>;
        using container_reference       = container_type&;
        using container_iterator        = container_type::iterator;
        using container_const_iterator  = container_type::const_iterator;

        /*-----------------------------------------------------------------------------*/
        /* Explicit Constructor                                                        */
        /*-----------------------------------------------------------------------------*/
        explicit TransformSystem(ECS_Manager& _ECS_Manager) : System{ _ECS_Manager } {};

        /*-----------------------------------------------------------------------------*/
        /* Lifecycle Functions                                                         */
        /*-----------------------------------------------------------------------------*/
        /****************************************************************************//*!
         @brief    Function that defines the initialization required for the
                   Runtime to function.
        *//*****************************************************************************/
        //void Init() /*override*/;

        /****************************************************************************//*!
         @brief    Function that defines the per-frame updates for the Runtime.
        *//*****************************************************************************/
        void Update() /*override*/;

        /****************************************************************************//*!
         @brief    Function that defines the clean up required for the Runtime
                   to close cleanly.
        *//*****************************************************************************/
        //void Exit() /*override*/;

        /****************************************************************************//*!
         @brief    Updates all transforms in a scene-graph manner recursively
        *//*****************************************************************************/
        //void UpdateTransform();

        //Scenegraph GetSceneGraph() { return m_scenegraph; }

      private:
        /****************************************************************************//*!
         @brief    private helper function that recursively updates all gameobjects in
                   a scene-graph(parent-child) manner.

         @param[in]    _go 
            the gameobject to update its local/global matrix and its children
         @param[in]    _parentTransform 
            the transform of the parent gameobject that will be used for global matrix
            calculations
        *//*****************************************************************************/
        //void updateTransform(GameObject& _go, glm::mat4 const * const _parentTransform);

        //Scenegraph m_scenegraph;

    };

} // namespace engine