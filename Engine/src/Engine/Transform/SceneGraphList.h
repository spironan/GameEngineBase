#pragma once

#include "Engine/ECS/ECS.h"
#include "Engine/ECS/GameObject.h"
#include "Engine/Transform/Transform3D.h"

namespace engine
{
    struct Scenegraph
    {
        //bool Attach(GameObject child, GameObject parent)
        //{
        //    int child_idx = child.IndexPosition();
        //    int parent_idx = parent.IndexPosition();

        //    if (child_idx == parent_idx) return false;    // attempting to add oneself to itself.
        //    
        //    // Child's number of Child
        //    int child_childCount = child.Transform().GetChildCount();
        //    // Perform a check to see if your'e attaching to a parent that is a child of child : 
        //    // Reject this , you cannot add a parent as a child to its child.
        //    if (child_idx < parent_idx && child_idx + child_childCount >= parent_idx) return false;

        //    // difference between their positions
        //    int diff = child_idx - parent_idx;
        //    //special case when its exactly 1 : Just attach and return
        //    if (diff == 1)
        //    {
        //        child.Transform().SetParent(parent.Transform());
        //        return;
        //    }

        //    // find direction to proceed from child to parent.
        //    bool proceedNegatively = diff > 0;

        //    // calculate number of times required to swap for each iteration
        //    diff = proceedNegatively ? diff - 1 : -diff;

        //    // find number of times to iterate : 1(itself) + once per child 
        //    int iterations = 1 + child_childCount;

        //    // iterate through and swap all relevant index(es)
        //    for(int currentIter = 0; currentIter < iterations; ++currentIter)
        //    {
        //        int start_idx = child_idx + (child_childCount - currentIter);
        //        int end_idx = parent_idx - currentIter;
        //        
        //        int swapCount = diff;
        //        while (swapCount--)
        //        {
        //            // find the next index
        //            int next_idx = proceedNegatively ? start_idx - 1 : start_idx + 1;
        //            // call the right version : swappign from sparse set
        //            std::swap(m_container[start_idx], m_container[next_idx]);
        //            start_idx = next_idx;
        //        }
        //    }

        //    // set child's parent to be parent transform
        //    child.Transform().SetParent(parent.Transform());

        //    return true;
        //}

    };
}