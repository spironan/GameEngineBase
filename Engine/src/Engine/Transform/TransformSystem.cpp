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
        // Algorithm that needs to happen! If this cannot be done, the scenegraph optimization suggestion following this will not be possible.
        // Assumptions : 
        // 1. Every Transform has a parent ID.
        // 2. Transform cannot actually "detach from parent" Transform always need to attach to something (to "detach" == attach to root node).
        // Sparse Set be ordered in the depth-first hierarchy, thus every add/remove of transform will need to adjust the sparse array
        // Position:    0      1       2        3
        // Example : [ root, child1, child1, child2]
        //                    ^parent       ^newly added child             
        // When attaching to initial parent : swap with index + 1 of the parent and the last position 
        // Position:    0      1       2        3
        // Example : [ root, child1, child2, child1 ]
        //                                  ^swapped child
        // 
        // Position:    0      1       2        3      4       5
        // Example : [ root, child1, child2, child3, child4, child1 ]
        //                   ^parent ^swapping to be child of root
        // When attaching to root : swap index of the parent(1) with its current index, and then continue doing that number of childrens times.
        // therefore : 1 swap with 2, 2 swap with 3, 3 swap with 4. swapping n times where n = number of children + 1.
        // Example : [ root, child1, child2, child3, child1, child1 ]
        //                   ^swapped child2         ^ the previous parent
        // 
        // Position:    0      1       2        3      4       5       6
        // Example : [ root, child1, child2, child3, child1, child2, child3 ]
        //                                    ^ new parent   ^child
        // When attaching from one sub-branch to another : swap index of the parent + 1 with its current index, and then continue doing that number of childrens times.
        // therefore : 4 swap with 5, swap 5 with 6
        // Example : [ root, child1, child2, child3, child4, child5, child1 ]
        //                                   ^parent  ^old child2
        // Special case 1 : When Attempting to add parent which it already is of, dont do anything.
        // 
        // Therefore from observation : 
        // 1. Should keep track of number of childrens
        // 2. transform MUST keep track of its parent's ID.
        // NOTE!
        // This could be done in 2 ways : Use additional memory to keep track of this list separately or apply this to the vector from the sparse set
        // Using a separate list gives way more power and flexibility at the cost of memory while vectors from the sparse set saves on that.
        // Using a linked list to keep track of their ID hierarchy allows for fast insertion, 
        // though this may lessen the point of using ecs as iteration on sparse set wont be used but the O(1) lookup is still being used

        //m_ECS_Manager.GetComponent<Transform3D>(ent);
        
        // Optimize Solution for Scenegraph
        // Assumptions That helps in general
        // 1. All Node Default parent is identity matrix
        // 2. Root Node will NEVER be adjusted
        // 3. Every element will always have a parent (removes if statements)
        // Requirements : Sparse Set be ordered in the depth-first hierarchy, thus every add/remove of transform will need to adjust the sparse array
        // Reason : we have to ENSURE that the parent is already recalculated first before doing this
        // Cost : Overhead when adding and removing a lot of entities
        // Benefit : significant improved performance due to not using recursion
        // example : [ root, child, child2, child2, child3, child, child, child, child2] 
        // Sample Code 
        // container_reference transforms = ComponentStore::GetComponents<Transform3D>();
        // for(auto& tf : transforms)
        // {
            ////recalculate if transform has been changed this frame
            //if (transform.IsDirty())
            //{
            //    transform.Recalculate();
            //}
            // calculated global transform : localTransform * parentTransform
            // by default its a transformation else it does coordinate conversion
            // transform.GlobalMatrix = transform.parent.globalTransform * localTransform;
        // }

            

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
        //updateTransform(root.child);
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
        // if go does not have transform do next
        if (!_go.HasComponent<Transform3D>()) return;

        Transform3D& transform = _go.GetComponent<Transform3D>();
        
        //recalculate if transform has been changed this frame
        if (transform.IsDirty())
        {
            transform.Recalculate();
        }

        //recalculate global transform if parent exist
        if (_parentTransform)
        {
            // calculated global transform : localTransform * parentTransform
            // by default its a transformation else it does coordinate conversion
            transform.SetGlobalMatrix(*_parentTransform); 
        }

        //depth first iterate through all children and update their transforms.
        for (auto& it : _go.GetChildren())
        {
            updateTransform(*it, &transform.GetGlobalMatrix());
            
            // did not manage to do this optimization due to the fact that updating the child
            // means that the matrix needs to be recalculated.
            // Solution if REQUIRED: let transform store a parent matrix and update it whenever
            // parent changes but if not use it for all calculations for global matrix.
            /*if (transform.HasChanged())
            {
                updateTransform(*it, &transform.GetGlobalMatrix());
            }
            else
            {
                updateTransform(*it, nullptr); 
            }*/
            //updateTransform
            //( *it
            ////, &transform.GetGlobalMatrix() 
            //, (transform.HasChanged()) 
            // ? &transform.GetGlobalMatrix() 
            // : nullptr
            //);
        }
    }

} // namespace engine