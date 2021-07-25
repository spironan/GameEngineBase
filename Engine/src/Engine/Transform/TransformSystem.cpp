/************************************************************************************//*!
\file           TransformSystem.cpp
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
#include "pch.h"

#include "Transform3D.h"
#include "TransformSystem.h"

#include "Engine/ECS/GameObject.h"
#include "Engine/ECS/ComponentManager.h"
#include "Engine/ECS/ECS_Manager.h"

namespace engine
{
    /*-----------------------------------------------------------------------------*/
    /* Lifecycle Functions                                                         */
    /*-----------------------------------------------------------------------------*/
    void TransformSystem::Init() { /* empty function */ }

    void TransformSystem::Update()
    {
        //container_reference transforms = ComponentStore::GetComponents<Transform3D>();
        auto view = m_ECS_Manager.GetComponentView<Transform3D>();
        
        // Iterate through and update all transforms that has changed last frame,
        // setting it to false.
        for (auto it : view)
        {
            auto& transform = m_ECS_Manager.GetComponent<Transform3D>(it);

            if (transform.HasChanged())
            {
                transform.Reset();
            }
        }

        UpdateTransform();
    }

    void TransformSystem::UpdateTransform() 
    {
        // recursively, depth first, update all gameobjects transforms in the scene.
        //GameObject& root = (getScene()->GetRootGameObject());
        //updateTransform(root, nullptr);
    }

    void TransformSystem::Exit() { /* empty function */ }
    

    /********************************************************************************//*!
     @brief    private helper function that recursively updates all gameobjects in
               a scene-graph(parent-child) manner.
     
     @param[in]    _go 
        the gameobject to update its local/global matrix and its children
     @param[in]    _parentTransform 
        the transform of the parent gameobject that will be used for global matrix
        calculations
    *//*********************************************************************************/
    void TransformSystem::updateTransform(GameObject& _go, glm::mat4 const * const _parentTransform)
    {
        //// if go does not have transform do next
        //if (!_go.HasComponent<Transform3D>()) return;

        //Transform3D& transform = _go.GetComponent<Transform3D>();
        //
        ////recalculate if transform has been changed this frame
        //if (transform.IsDirty())
        //{
        //    transform.Recalculate();
        //}

        ////recalculate global transform if parent exist
        //if (_parentTransform)
        //{
        //    // calculated global transform : localTransform * parentTransform
        //    // by default its a transformation else it does coordinate conversion
        //    transform.SetGlobalMatrix(*_parentTransform); 
        //}

        ////depth first iterate through all children and update their transforms.
        //for (auto& it : _go.GetChildren())
        //{
        //    updateTransform(*it, &transform.GetGlobalMatrix());
        //    
        //    // did not manage to do this optimization due to the fact that updating the child
        //    // means that the matrix needs to be recalculated.
        //    // Solution if REQUIRED: let transform store a parent matrix and update it whenever
        //    // parent changes but if not use it for all calculations for global matrix.
        //    /*if (transform.HasChanged())
        //    {
        //        updateTransform(*it, &transform.GetGlobalMatrix());
        //    }
        //    else
        //    {
        //        updateTransform(*it, nullptr); 
        //    }*/
        //    //updateTransform
        //    //( *it
        //    ////, &transform.GetGlobalMatrix() 
        //    //, (transform.HasChanged()) 
        //    // ? &transform.GetGlobalMatrix() 
        //    // : nullptr
        //    //);
        //}
    }

} // namespace engine