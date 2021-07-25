/************************************************************************************//*!
\file           Transform3D.cpp
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
#include "pch.h"

#include "Transform3D.h"
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine 
{
    /********************************************************************************//*!
     @brief    Overloaded constructor for transform component
     
     @param[in]    _entityID 
        The id that is attached to the entity.
     
     @param[in]    _active 
        Determines if the component is active or not.
    *//*********************************************************************************/
    Transform3D::Transform3D(Entity _entityID, bool _active)
    : Component         { _entityID, _active }
    , position          { 0.f, 0.f, 0.f }
    , rotation          { 0.f, 0.f, 0.f }
    , scale             { 1.f, 1.f, 1.f }
    , conversion        { false }
    , dirty             { false }
    , hasChanged        { false }
    , globalTransform   { }
    , localTransform    { }
    , conversionMtx     { }
    {
    }

    /****************************************************************************//*!
     @brief    Recalculate the local transform matrix.
               Should only be called by the transform runtime
    *//*****************************************************************************/
    void Transform3D::Recalculate()
    {
        //localTransform = Matrix_util::model_matrix(position, rotation, scale);
        glm::translate(localTransform, position);
        //glm::rotate(localTransform, rotation);
        //glm::rotate(localTransform, );
        glm::scale(localTransform, scale);

        //Apply conversion everytime upon calculation
        conversionMtx *= localTransform;

        //it is now recalculated and clean.
        dirty = false;

        globalTransform = localTransform;

        //it has changed and been updated this frame
        hasChanged = true;
    }

    /****************************************************************************//*!
     @brief    Sets the Global transform matrix.
               Should only be called by the transform runtime

     @param[in]    _parentTransform
        the parent matrix to multiply with to generate global Transform3D matrix.
    *//*****************************************************************************/
    void Transform3D::SetGlobalMatrix(glm::mat4 _parentTransform)
    {
        if (conversion)
        {
            // store parents inverse as conversion matrix
            conversionMtx = glm::inverse(_parentTransform);
            
            //recalculate again
            dirty       = true;
            conversion  = false;
        }

        globalTransform = _parentTransform * localTransform;
        
    }
    /****************************************************************************//*!
     @brief    Retrieves the global rotation matrix of this object from the global
               transformation matrix.

     @warning  Take note that in most cases, unless being called after the Transform3D
               Runtime has been executed, this will be the position from the previous
               frame.

     @return   An AEMtx33 that represents the current rotation matrix
               of this GameObject in global coordinates.
    *//*****************************************************************************/
    glm::mat4 Transform3D::GetGlobalRotationMatrix() const
    {
        glm::mat4 result{ globalTransform };
        result[0][2] = 0.0f;
        result[1][2] = 0.0f;
        glm::vec3 scalar{ GetGlobalScale() };

        //TAKE NOTE OF THE AXIS IT MULTIPLIES BY ( EACH ROW MULTIPLY BY THAT ROW's SCALAR)
        result[0][0] /= scalar[0];
        result[1][0] /= scalar[0];
        result[2][0] /= scalar[0];

        result[0][1] /= scalar[1];
        result[1][1] /= scalar[1];
        result[2][1] /= scalar[1];

        result[0][2] /= scalar[2];
        result[1][2] /= scalar[2];
        result[2][2] /= scalar[2];
        
        return result;
    }
    
    /*float Transform3D::GetGlobalRotationDeg() const
    {
        return GetGlobalRotationRad() * 180.f / PI;
    }

    float Transform3D::GetGlobalRotationRad() const
    {
        return std::atan2f(-globalTransform[0][1], globalTransform[0][0]);
    }*/

    /****************************************************************************//*!
     @brief    Retrieves the global scale of this object from the global
               transformation matrix.

     @warning  Take note that in most cases, unless being called after the Transform3D
               Runtime has been executed, this will be the position from the previous
               frame.

     @return   An AEVec2 that represents the current scale of this GameObject in
               global coordinates.
    *//*****************************************************************************/
    glm::vec3 Transform3D::GetGlobalScale() const
    {
        //calculate global scale by calculating the length of each row which represents
        //the scale of that particular axis.
        glm::vec3 vecX { globalTransform[0][0], globalTransform[1][0], globalTransform[2][0] };
        glm::vec3 vecY { globalTransform[0][1], globalTransform[1][1], globalTransform[2][1] };
        glm::vec3 vecZ { globalTransform[0][1], globalTransform[1][1], globalTransform[2][2] };

        return glm::vec3{ glm::length(vecX), glm::length(vecY), glm::length(vecZ) };
    }

} // namespace engine