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
    , m_position          { 0.f, 0.f, 0.f }
    , m_rotationAngle     { 0.f }
    , m_rotationAxis      { 0.f, 0.f, 1.f }
    , m_scale             { 1.f, 1.f, 1.f }
    , m_conversion        { false }
    , m_dirty             { false }
    , m_hasChanged        { false }
    , m_globalTransform   { glm::mat4{} }
    , m_localTransform    { glm::mat4{} }
    , m_conversionMatrix     { glm::mat4{} }
    {
    }

    /****************************************************************************//*!
     @brief    Recalculate the local transform matrix.
               Should only be called by the transform runtime
    *//*****************************************************************************/
    void Transform3D::Recalculate()
    {
        //localTransform = Matrix_util::model_matrix(position, rotation, scale);
        glm::translate(m_localTransform, m_position);
        glm::rotate(m_localTransform, m_rotationAngle, m_rotationAxis);
        //glm::rotate(localTransform, )
        //glm::rotate(localTransform, rotation);
        //glm::rotate(localTransform, );
        glm::scale(m_localTransform, m_scale);

        //Apply conversion everytime upon calculation
        m_conversionMatrix *= m_localTransform;

        //it is now recalculated and clean.
        m_dirty = false;

        m_globalTransform = m_localTransform;

        //it has changed and been updated this frame
        m_hasChanged = true;
    }

    /****************************************************************************//*!
     @brief    Sets the Global transform matrix.
               Should only be called by the transform runtime

     @param[in]    _parentTransform
        the parent matrix to multiply with to generate global Transform3D matrix.
    *//*****************************************************************************/
    void Transform3D::SetGlobalMatrix(glm::mat4 _parentTransform)
    {
        if (m_conversion)
        {
            // store parents inverse as conversion matrix
            m_conversionMatrix = glm::inverse(_parentTransform);
            
            //recalculate again
            m_dirty       = true;
            m_conversion  = false;
        }

        m_globalTransform = _parentTransform * m_localTransform;
        
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
        glm::mat4 result{ m_globalTransform };
        result[0][3] = 0.0f;
        result[1][3] = 0.0f;
        result[2][3] = 0.0f;
        glm::vec3 scalar{ GetGlobalScale() };

        /*result[0] /= scalar[0];
        result[1] /= scalar[1];
        result[2] /= scalar[2];

        return glm::transpose(result);*/

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
        glm::vec3 vecX { m_globalTransform[0][0], m_globalTransform[1][0], m_globalTransform[2][0] };
        glm::vec3 vecY { m_globalTransform[0][1], m_globalTransform[1][1], m_globalTransform[2][1] };
        glm::vec3 vecZ { m_globalTransform[0][1], m_globalTransform[1][1], m_globalTransform[2][2] };
        return glm::vec3{ glm::length(vecX), glm::length(vecY), glm::length(vecZ) };
        
        //glm::transpose(globalTransform);
        //return glm::vec3{ globalTransform[0].length(), globalTransform[1].length(), globalTransform[2].length() };
    }

} // namespace engine