/************************************************************************************//*!
\file           TransformSystem.h
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
#pragma once

#include "Engine/ECS/System.h"

namespace engine 
{
    //forward declaration
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
         @brief
        *//*****************************************************************************/
        void Store(Entity entity);
        
        /****************************************************************************//*!
         @brief    
        *//*****************************************************************************/
        void Restore(Entity entity);

        /****************************************************************************//*!
         @brief    Function that assume the dense array is ordered properly in hierarchy
                   and updates all transforms to make sure that it is. 
                   If the assumption is not true, this function will produce breaking
                   behaviour. USE WITH GREAT CARE AND AWARENESS.

         @caution  Use this Function with care and make sure you know exactly what is
                   you're doing. Inappropriate use will cause UDB and break the whole
                   engine.
        *//*****************************************************************************/
        void UseDenseArrayAsHierarchy();

        /****************************************************************************//*!
         @brief    Function that defines the per-frame updates for the Runtime.
        *//*****************************************************************************/
        void Update();

        /****************************************************************************//*!
         @brief    Retrieves all transform components of a given transform
        *//*****************************************************************************/
        std::vector<Transform3D> GetChildren(Transform3D const& transform) const;

        /****************************************************************************//*!
         @brief    Updates all transforms in a scene-graph manner recursively
        *//*****************************************************************************/
        void UpdateTransform();

        /****************************************************************************//*!
         @brief     Attaches child GameObject to Parent GameObject in a scenegraph manner

         @param     child
                the node that will potential become a child if operation succeed.
         @param     parent
                the node that will potentially become a parent node if operation succeed.

         @return    Returns true if the attach succeeded. False otherwise.
                    In General, attaching will succeed as long as
                    1. the child is not the root node(first node)
                    2. you're not trying to attach oneself to itself
                    3. youre not trying to attach oneself to your own children.
        *//*****************************************************************************/
        bool Attach(Entity child, Entity parent);

    private:
        Entity m_restoreLocation = -1;
    };

} // namespace engine