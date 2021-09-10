/************************************************************************************//*!
\file           TransformSystem.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 22, 2021
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

#include "Engine/ECS/ComponentManager.h"
#include "Engine/ECS/ECS_Manager.h"

#include "Engine/ECS/GameObject.h"


namespace engine
{
    /*-----------------------------------------------------------------------------*/
    /* Lifecycle Functions                                                         */
    /*-----------------------------------------------------------------------------*/

    void TransformSystem::Update()
    {
        // Algorithm that needs to happen! If this cannot be done, the scenegraph optimization suggestion following this will not be possible.
        // Assumptions : 
        // 1. Every Transform has a parent Entity ID.
        // 2. Transform cannot actually "detach from parent" Transform always need to attach to something (to "detach" == attach to root node).
        // 3. If parent dies all residue children dies.
        // Sparse Set be ordered in the depth-first hierarchy, thus every add/remove of transform will need to adjust the sparse array
        // Therefore from observation : 
        // 1. Should keep track of number of childrens
        // 2. transform MUST keep track of its parent's ID.
        // NOTE!
        // This could be done in 2 ways : Use additional memory to keep track of this list separately or apply this to the vector from the sparse set
        // Using a separate list gives way more power and flexibility at the cost of memory while vectors from the sparse set saves on that.
        // Using a linked list to keep track of their ID hierarchy allows for fast insertion, 
        // though this may lessen the point of using ecs as iteration on sparse set wont be used but the O(1) lookup is still being used

        // Optimize Solution for Scenegraph
        // Assumptions That helps in general
        // 1. All Node Default parent is identity matrix
        // 2. Root Node will NEVER be adjusted
        // 3. Every element will always have a parent (removes if statements)
        // Requirements : Sparse Set be ordered in the depth-first hierarchy, thus every add/remove of transform will need to adjust the sparse array
        // Reason : we have to ENSURE that the parent is already recalculated first before doing this
        // Cost : Overhead when adding and removing a lot of entities
        // Benefit : significant improved performance due to not using recursion
        // example : [ root, child, child2, child2, child3, child, child, child, child2 ] 
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

        // ASSUMPTION 0 : ALL GAMEOBJECTS MUST HAVE A TRANSFORM COMPONENT.
        // ASSUMPTION 1 : ROOT NODE MUST BE THE FIRST NODE
        // ASSUMPTION 2 : ALL NODES HAVE A PARENT
        // 
        // Iterate through and update all transforms that has changed last frame,
        // setting it to false.
        
        // Grab Transforms from ECS manager
        for (auto& currTransform : m_ECS_Manager.GetComponentDenseArray<Transform3D>())
        {
            if (!currTransform.m_active) continue;

            //m_ECS_Manager.GetComponent<GameObjectComponent>()

            if (currTransform.HasChanged())
            {
                currTransform.Reset();
            }
            
            //recalculate if transform has been changed this frame
            if (currTransform.IsDirty())
            {
                currTransform.Recalculate();
            }

            //Always update the global transform
            // for the root node it'll just update itself against Identity thus nothing happens.
            // Recalculate global transform : Assumption is that parent ALWAYS Exist.
            // calculated global transform : localTransform * parentTransform
            // by default its a transformation else it does coordinate conversion
            currTransform.SetGlobalMatrix();
        }

    }

    // Grab Transforms from ECS manager and assumes its ordered correctly and child count is correct.
    void TransformSystem::UseDenseArrayAsHierarchy()
    {
        auto& tfDense = m_ECS_Manager.GetComponentDenseArray<Transform3D>();
        auto& root = tfDense.front();
        std::stack<Entity> parentStack;
        for (int i = 1; i < tfDense.size(); ++i) parentStack.emplace(root.GetEntity());

        for (auto& iter = tfDense.begin() + 1; iter != tfDense.end(); ++iter)
        {
            iter->m_parentId = parentStack.top();
            parentStack.pop();
            if (iter->m_childCount > 0)
            {
                for (int i = 0; i < iter->GetChildCount(); ++i)
                    parentStack.emplace(iter->GetEntity());
            }
        }
    }

    // Grab Transforms from ECS manager and assumes its ordered correctly and parent id is correct
    void TransformSystem::RefreshHierarchy()
    {
        auto& root = m_ECS_Manager.GetComponentDenseArray<Transform3D>().front();

        for (auto& iter : m_ECS_Manager.GetComponentDenseArray<Transform3D>())
        {
            m_ECS_Manager.GetComponent<Transform3D>(iter.m_parentId).m_childCount += 1;
        }
    }

    void TransformSystem::Store(Entity entity)
    {
        //auto& tf = m_ECS_Manager.GetComponent<Transform3D>(entity);
        
        // starting location in the dense array to recover to
        //m_restoreLocation = m_ECS_Manager.GetComponentContainer<Transform3D>().GetIndex(entity);

        m_restorationStack.emplace(m_ECS_Manager.GetComponentContainer<Transform3D>().GetIndex(entity));

        // store all the 
        //m_ECS_Manager.GetComponentContainer<Transform3D>().;

    }

    void TransformSystem::Restore(Entity entity)
    {
        if (m_restorationStack.size() == 0) return;
        //if (m_restoreLocation == -1) return;

        m_ECS_Manager.GetComponentContainer<Transform3D>().ShiftElementsFromBackToIndex(m_restorationStack.top(),
            m_ECS_Manager.GetComponentContainer<Transform3D>().GetIndex(entity));
        RefreshHierarchy();
        //UseDenseArrayAsHierarchy();
        //m_restoreLocation = -1;
        m_restorationStack.pop();
    }


    std::vector<Transform3D> TransformSystem::GetChildren(Transform3D const& transform) const
    {
        //seems like possible performance dip? idk or visual studio being visual studio
        // [TODO] REMEMBER TO change to const&
        auto& sparseSetContainer = m_ECS_Manager.GetComponentContainer<Transform3D>();
        size_t startIdx = sparseSetContainer.GetIndex(transform.m_entity) + 1;
        size_t endIdx = startIdx + transform.GetChildCount();
        return std::vector<Transform3D>(sparseSetContainer.begin() + startIdx, sparseSetContainer.begin() + endIdx);
    }

    void TransformSystem::UpdateTransform()
    {
        // Grab Transforms from ECS manager
        for (auto& currTransform : m_ECS_Manager.GetComponentDenseArray<Transform3D>())
        {
            //recalculate if transform has been changed this frame
            if (currTransform.IsDirty())
            {
                currTransform.Recalculate();
            }

            //Always update the global transform
            // for the root node it'll just update itself against Identity thus nothing happens.
            // Recalculate global transform : Assumption is that parent ALWAYS Exist.
            // calculated global transform : localTransform * parentTransform
            // by default its a transformation else it does coordinate conversion
            currTransform.SetGlobalMatrix();
        }
    }

    /****************************************************************************//*!
      @brief  Attaches child GameObject to Parent GameObject in a scenegraph manner
              Allows data to be laid in a way that one can do iteration
              for traversing scenegraph instead of recursion.
      
      @param[in] child
          the node that will potential become a child if operation succeed.
      @param[in] parent
          the node that will potentially become a parent node if operation succeed.
      
      @return    Returns true if the attach succeeded. False otherwise.
              In General, attaching will succeed as long as
              1. the child is not the root node(first node)
              2. you're not trying to attach oneself to itself
              3. youre not trying to attach oneself to your own children.
    *//*****************************************************************************/
    bool TransformSystem::Attach(Entity child, Entity parent)
    {
        int child_idx   = static_cast<int>(m_ECS_Manager.GetComponentContainer<Transform3D>().GetIndex(child));
        int parent_idx  = static_cast<int>(m_ECS_Manager.GetComponentContainer<Transform3D>().GetIndex(parent));

        if (child_idx == parent_idx) return false;    // attempting to add oneself to itself.
        
        Transform3D& childTf = m_ECS_Manager.GetComponent<Transform3D>(child);

        // Child's number of Child
        int child_childCount = childTf.GetChildCount();
        // Perform a check to see if your'e attaching to a parent that is a child of child : 
        // Reject this , you cannot add a parent as a child to its child.
        if (child_idx < parent_idx && child_idx + child_childCount >= parent_idx) return false;

        Transform3D& parentTf = m_ECS_Manager.GetComponent<Transform3D>(parent);

        // set child's parent to be parent transform
        childTf.SetParent(parentTf);

        // difference between their positions
        int diff = child_idx - parent_idx;
        //special case when its exactly 1 : nothing more needs to be done.
        if (diff == 1)
        {
            return true;
        }

        // reference to container
        auto& container = m_ECS_Manager.GetComponentContainer<Transform3D>();

        // find direction to proceed from child to parent.
        bool proceedNegatively = diff > 0;

        // calculate number of times required to swap for each iteration
        //diff = proceedNegatively ? diff - 1 : -diff;

        // find number of times to iterate : 1(itself) + once per child 
        int iterations = 1 + child_childCount;

        // iterate through and swap all relevant index(es)
        for(int currentIter = 0; currentIter < iterations; ++currentIter)
        {
            // Calculate start and end index depending on whether we are 
            // iteratively swapping forward or backwards based on the diff and current
            // iteration.
            int start_idx = child_idx + child_childCount - !proceedNegatively * currentIter;
            int end_idx = parent_idx - !proceedNegatively * currentIter;

            // calculate number of times to swap
            int swapCount = proceedNegatively ? start_idx - end_idx - 1 : end_idx - start_idx;
            while (swapCount--)
            {
                // find the next index
                int next_idx = proceedNegatively ? start_idx - 1 : start_idx + 1;
                // swap container indexes
                container.Swap(start_idx, next_idx);
                // set starting index to be the next and continue to next iteration
                start_idx = next_idx;
            }
        }

        return true;
    }


} // namespace engine