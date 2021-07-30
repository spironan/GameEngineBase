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

#include "Engine/ECS/GameObject.h"
#include "Engine/ECS/ComponentManager.h"
#include "Engine/ECS/ECS_Manager.h"

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
        //             ^parent        ^swapping to be child of root
        // When attaching to root : swap index of the index + 1 of the parent(1) with its current index, and then continue doing that number of childrens times.
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
        // 
        // Position:    0      1       2        3      4       5       6
        // Example : [ root, child1, child2, child3, child1, child2, child3 ]
        //                                   ^child          ^ new parent         
        // When attaching from one node to the LAST node : swap index of child + 1 with child index, and then continue doing so until child index > parent index
        // therefore : 4 swap with 5, swap 5 with 6
        // Example : [ root, child1, child2, child1, child2, child3, child3 ]
        //                                   ^ parent         ^ old child3
        //
        // Position:    0      1       2        3      4       5       6
        // Example : [ root, child1, child2, child3, child1, child2, child3 ]
        //                          ^child                            ^ new parent
        // When attaching from one node to the LAST node : swap index of child + 1 with child index, and then continue doing so until child index > parent index
        // therefore : 4 swap with 5, swap 5 with 6
        // Example : [ root, child1, child2, child1, child2, child3, child3 ]
        //                                   ^ parent         ^ old child3
        // 
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
    bool TransformSystem::Attach(GameObject const& child, GameObject const& parent)
    {
        int child_idx   = static_cast<int>(m_ECS_Manager.GetComponentContainer<Transform3D>().GetIndex(child));
        int parent_idx  = static_cast<int>(m_ECS_Manager.GetComponentContainer<Transform3D>().GetIndex(parent));

        if (child_idx == parent_idx) return false;    // attempting to add oneself to itself.
        
        // Child's number of Child
        int child_childCount = child.Transform.GetChildCount();
        // Perform a check to see if your'e attaching to a parent that is a child of child : 
        // Reject this , you cannot add a parent as a child to its child.
        if (child_idx < parent_idx && child_idx + child_childCount >= parent_idx) return false;

        // set child's parent to be parent transform
        child.Transform.SetParent(parent.Transform);

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