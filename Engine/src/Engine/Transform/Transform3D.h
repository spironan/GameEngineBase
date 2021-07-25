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
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

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
        Transform3D(const Transform3D&)             = default;
        Transform3D& operator=(const Transform3D&)  = default;
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
        inline const glm::vec3  GetPosition()       const   { return position; }
        inline const glm::vec3  GetRotation()       const   { return rotation; }
        inline const glm::vec3  GetScale()          const   { return scale;    }

        inline const glm::mat4& GetLocalMatrix()    const   { return localTransform;}
        inline const glm::mat4& GetGlobalMatrix()   const   { return globalTransform; }
        /****************************************************************************//*!
         @brief    Retrieves the global position of this Component from the global
                   transformation matrix.

         @warning  Take note that in most cases, unless being called after the Transform
                   Runtime has been executed, this will be the position from the previous
                   frame.

         @return   An AEVec2 that represents the previous position of this Component in
                   global coordinates.
        *//*****************************************************************************/
        inline glm::vec3 GetGlobalPosition() const { return { globalTransform[0][2], globalTransform[1][2], globalTransform[2][2] }; }
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
        glm::vec3                GetGlobalScale() const;

        inline const bool   IsDirty()           const   { return dirty;    }
        inline const bool   HasChanged()        const   { return hasChanged; }

        /*-----------------------------------------------------------------------------*/
        /* Setter Functions                                                            */
        /*-----------------------------------------------------------------------------*/
        inline glm::vec3&   Position()      { dirty = true; return position; }
        inline glm::vec3&   Rotation()      { dirty = true; return rotation; }
        inline glm::vec3&   Scale()         { dirty = true; return scale;    }
        inline void         Reset()         { hasChanged = false; }

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
        inline void ConvertCoordinates() { dirty = true; conversion = true; }
        
      private:

        glm::mat4 globalTransform;
        glm::mat4 localTransform;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        glm::mat4 conversionMtx;
        bool conversion; 

        bool dirty;
        bool hasChanged;

    };

} // namespace Engine