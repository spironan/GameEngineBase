/************************************************************************************//*!
\file           Transform3D.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 07, 2021
\brief          

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/ECS/Component.h"
#include <glm/glm.hpp>

namespace engine 
{ 
    /********************************************************************************//*!
     @brief    Defines a 2D transform component. A Transform Component is used to
               identify a position in space and how much to rotate and scale the 
               object by.
               Refer to this as a template of how to implement a component.
    *//*********************************************************************************/
    class Transform3D final : public Component
    {
      public:
        
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        Transform3D()                               = delete;
        Transform3D(Transform3D const&)             = default;
        Transform3D(Transform3D &&)                 = default;
        Transform3D& operator=(Transform3D const&)  = default;
        Transform3D& operator=(Transform3D &&)      = default;
        virtual ~Transform3D() override             = default;

        /****************************************************************************//*!
         @brief    Overloaded constructor for transform component
         
         @param[in]    _entityID 
            The id that is attached to the entity.
         @param[in]    _active 
            Determines if the component is active or not.
        *//*****************************************************************************/
        Transform3D(Entity _entityID, bool _active = true);

        
        /*-----------------------------------------------------------------------------*/
        /* Getter Functions                                                            */
        /*-----------------------------------------------------------------------------*/
        const float      GetRotationAngle()  const   { return m_rotationAngle; }
        const glm::vec3  GetPosition()       const   { return m_position; }
        const glm::vec3  GetRotationAxis()   const   { return m_rotationAxis; }
        const glm::vec3  GetScale()          const   { return m_scale;    }

        const glm::mat4& GetLocalMatrix()    const   { return m_localTransform;}
        const glm::mat4& GetGlobalMatrix()   const   { return m_globalTransform; }

        const int&       GetChildCount()     const   { return m_childCount;}
        const Entity&    GetParentId()       const   { return m_parentId;}
        /****************************************************************************//*!
         @brief    Retrieves the global position of this Component from the global
                   transformation matrix.

         @warning  Take note that in most cases, unless being called after the Transform
                   Runtime has been executed, this will be the position from the previous
                   frame.

         @return   An AEVec2 that represents the previous position of this Component in
                   global coordinates.
        *//*****************************************************************************/
        glm::vec3 GetGlobalPosition() const { return { m_globalTransform[0][3], m_globalTransform[1][3], m_globalTransform[2][3] }; }
        /****************************************************************************//*!
         @brief    Retrieves the global rotation matrix of this object from the global
                   transformation matrix.
                   Link to explaination : https://tinyurl.com/deriveTRS

         @warning  Take note that in most cases, unless being called after the Transform
                   Runtime has been executed, this will be the rotation matrix 
                   from the previous frame.

         @return   An AEMtx33 that represents the rotation matrix from the previous frame
                   of this Component in global coordinates.
        *//*****************************************************************************/
        glm::mat4 GetGlobalRotationMatrix() const;

        ///****************************************************************************//*!
        // @brief    Retrieves the global rotation of this object in radians 
        //           from the global transformation matrix.
        //           link to explaination : https://tinyurl.com/DeriveTRSfrom2dMat

        // @warning  Take note that in most cases, unless being called after the Transform
        //           Runtime has been executed, this will be the rotation from the previous
        //           frame.

        // @return   A float in radians that represents the rotation angle of the previous 
        //           frame of this Component in global coordinates.
        //*//*****************************************************************************/
        //float GetGlobalRotationRad() const;

        ///****************************************************************************//*!
        // @brief    Retrieves the global rotation of this object in degress
        //           from the global transformation matrix.
        //           link to explaination : https://tinyurl.com/DeriveTRSfrom2dMat

        // @warning  Take note that in most cases, unless being called after the Transform
        //           Runtime has been executed, this will be the rotation from the previous
        //           frame.

        // @return   A float in degrees that represents the rotation angle of the previous 
        //           frame of this Component in global coordinates.
        //*//*****************************************************************************/
        //float GetGlobalRotationDeg() const;

        /****************************************************************************//*!
         @brief    Retrieves the global scale of this object from the global
                   transformation matrix.
                   Link to explaination : https://tinyurl.com/deriveTRS

         @warning  Take note that in most cases, unless being called after the Transform
                   Runtime has been executed, this will be the scale from the previous
                   frame.

         @return   An AEVec2 that represents the previous scale of this GameObject in
                   global coordinates.
        *//*****************************************************************************/
        glm::vec3    GetGlobalScale() const;

        const bool   IsDirty()           const   { return m_dirty;    }
        const bool   HasChanged()        const   { return m_hasChanged; }

        /*-----------------------------------------------------------------------------*/
        /* Setter Functions                                                            */
        /*-----------------------------------------------------------------------------*/
        float&       RotationAngle() { m_dirty = true; return m_rotationAngle; }
        glm::vec3&   Position()      { m_dirty = true; return m_position; }
        glm::vec3&   RotationAxis()  { m_dirty = true; return m_rotationAxis; }
        glm::vec3&   Scale()         { m_dirty = true; return m_scale;    }
        void         Reset()         { m_hasChanged = false; }


        /*-----------------------------------------------------------------------------*/
        /* Member Functions                                                            */
        /*-----------------------------------------------------------------------------*/

        /****************************************************************************//*!
         @brief    Recalculate the local transform matrix.
                   Should only be called by the transform runtime
        *//*****************************************************************************/
        void Recalculate();

        /****************************************************************************//*!
         @brief    Sets the Global transform matrix.
                   Should only be called by the transform runtime

         @param[in]    _parentTransform
            the parent matrix to multiply with to generate global Transform matrix.
        *//*****************************************************************************/
        void SetGlobalMatrix(glm::mat4 _parentTransform);

        /****************************************************************************//*!
         @brief    Tells the Transform to do coordinate conversion instead of 
                   position transformation when attaching to a parent that is not
                   the root gameobject.
                   Call this function when you want to attach a new parent to prevent
                   the object from being geometrically moved in space.

         @note     Conversion will automatically be set back to false once there is
                   the conversion matrix has been calculated.
        *//*****************************************************************************/
        void ConvertCoordinates() { m_dirty = true; m_conversion = true; }
        

        void SetParent(Transform3D& parent);

    private:

        int m_childCount;
        Entity m_parentId;

        glm::mat4 m_globalTransform;
        glm::mat4 m_localTransform;

        float m_rotationAngle;

        glm::vec3 m_position;
        glm::vec3 m_rotationAxis;
        glm::vec3 m_scale;

        glm::mat4 m_conversionMatrix;
        bool m_conversion; 

        bool m_dirty;
        bool m_hasChanged;
    };

} // namespace Engine