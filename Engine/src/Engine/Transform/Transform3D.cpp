/************************************************************************************//*!
\file           Transform3D.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 22, 2021
\brief          Defines a 2D transform component. A Transform Component is used to
                identify a position in space and how much to rotate and scale the 
                object by.
                Refer to this as a template of how to implement a component.
                Implements RTTR as well.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"

#include "Transform3D.h"
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <rttr/registration>
#include "Engine/ECS/GameObject.h"

#include "TransformSystem.h"

namespace engine 
{
    /********************************************************************************//*!
     @brief     Used to register for RTTR Specifically to display the relevant 
                information that will be displayed to the editor
    *//*********************************************************************************/
    RTTR_REGISTRATION
    {
        using namespace rttr;
        registration::class_<Transform3D>("Transform3D")
            .property("Position", &Transform3D::GetPosition, &Transform3D::SetPosition)
            .property_readonly("Rotation Axis", &Transform3D::GetRotationAxis)
            .property("Rotation Angle", &Transform3D::GetRotationAngle, &Transform3D::SetRotationAngle)
            .property("Scaling", &Transform3D::GetScale, &Transform3D::SetScale)
            .property_readonly("My ID", &Transform3D::GetID)
            .property_readonly("Parent ID",&Transform3D::GetParentId)
            .property_readonly("No Of Childs", &Transform3D::GetChildCount)
            .property_readonly("Local Matrix", &Transform3D::GetLocalMatrix)
            .property_readonly("Global Matrix", &Transform3D::GetGlobalMatrix)
            .property_readonly("Global Rotation", &Transform3D::GetGlobalRotationDeg)
            .property_readonly("Conversion Matrix", &Transform3D::GetConversionMatrix); //added readonly for debugging purposes
    }

    /********************************************************************************//*!
     @brief    Overloaded constructor for transform component
     
     @param[in]    _entityID 
        The id that is attached to the entity.
     
     @param[in]    _active 
        Determines if the component is active or not.
    *//*********************************************************************************/
    Transform3D::Transform3D(Entity entityID, bool active)
        : Component         { entityID, active }
        , m_position        { 0.f, 0.f, 0.f }
        , m_rotationAngle   { 0.f }
        , m_rotationAxis    { 0.f, 0.f, 1.f }
        , m_scale           { 1.f, 1.f, 1.f }
        , m_conversion      { false }
        , m_dirty           { false }
        , m_hasChanged      { false }
        , m_globalTransform { oom::mat4{ 1.f } }
        , m_localTransform  { oom::mat4{ 1.f } }
        , m_conversionMatrix{ oom::mat4{ 1.f } }
        , m_childCount      { 0 }
        , m_parentId        { entityID }
    {
    }
    
    /****************************************************************************//*!
     @brief    Recalculate the local transform matrix.
               Should only be called by the transform runtime
    *//*****************************************************************************/
    void Transform3D::Recalculate()
    {
        oom::mat4 scale = oom::scale(oom::mat4{ 1.f }, m_scale);
        oom::mat4 rot = oom::rotate(oom::mat4{ 1.f }, m_rotationAngle, m_rotationAxis);
        oom::mat4 trans = oom::translate(oom::mat4{ 1.f }, m_position);

        m_localTransform = trans * rot * scale;

        //Apply conversion everytime upon calculation
        m_localTransform = m_conversionMatrix * m_localTransform;

        //it is now recalculated and clean.
        m_dirty = false;

        m_globalTransform = m_localTransform;

        //it has changed and been updated this frame
        m_hasChanged = true;
    }

    /****************************************************************************//*!
     @brief    Sets the Global transform matrix.
               Should only be called by the transform runtime
    *//*****************************************************************************/
    void Transform3D::SetGlobalMatrix()
    {
        if (m_parentId == m_entity) return;

        oom::mat4 parentMtx = static_cast<GameObject>(m_parentId).Transform().GetGlobalMatrix();

        if (m_conversion)
        {
            // store parents inverse as conversion matrix
            m_conversionMatrix = oom::inverse(parentMtx).value();
            
            //recalculate again
            m_dirty       = true;
            m_conversion  = false;
        }
        
        m_globalTransform = parentMtx * m_localTransform;
    }

    void engine::Transform3D::DetachFromRoot()
    {
        // Reduce child count of current parent if parent isnt itself.
        if (m_parentId != m_entity)
        {
            static_cast<GameObject>(m_parentId).Transform().DecrementChildCount(1 + m_childCount);
        }

        //m_parentId = m_entity;

        /*SetActive(false);
        auto& result = WorldManager::GetActiveWorld().GetSystem<engine::TransformSystem>()->GetChildren(*this);

        return result;*/
    }

    /****************************************************************************//*!
     @brief     Assign this transform parent class

     @param[in]    parent
            The new parent of this transform class
    *//*****************************************************************************/
    void Transform3D::SetParent(Transform3D& parent)
    {
        // Reduce child count of current parent if parent isnt itself.
        if (m_parentId != m_entity)
        {
            static_cast<GameObject>(m_parentId).Transform().DecrementChildCount(1 + m_childCount);
        }
        // set parent child count to be equals to its current amount + 1(this object) + childCount(number of children this object has)
        parent.IncrementChildCount(1 + m_childCount);
        // set this parent id to be the entity ID of the parent
        m_parentId = parent.m_entity;
    }

    /****************************************************************************//*!
     @brief     Helper function to recursively increment child count of all
                parent nodes.

     @param    childCount
                the number of children to increment the new parent's child count by.
    *//*****************************************************************************/
    void Transform3D::IncrementChildCount(int childCount)
    {
        m_childCount += childCount;
        if (m_parentId != m_entity)
        {
            static_cast<engine::GameObject>(m_parentId).Transform().IncrementChildCount(childCount);
        }
    }

    /*********************************************************************************//*!
     @brief     Helper function to recursively decrement child count of all
                parent nodes.

     @param    childCount
                the number of children to decrement the new parent's child count by.
    *//**********************************************************************************/
    void Transform3D::DecrementChildCount(int childCount)
    {
        m_childCount -= childCount;
        if (m_parentId != m_entity)
        {
            static_cast<engine::GameObject>(m_parentId).Transform().DecrementChildCount(childCount);
        }
    }

    /****************************************************************************//*!
     @brief     Copies the relevant data of transform component while retaining
                its entityID to be of prior gameobject before this operation.
    *//*****************************************************************************/
    Component& Transform3D::CopyComponent(Component const& comp)
    {
        auto& transform = reinterpret_cast<Transform3D const&>(comp);
        m_localTransform    = transform.m_localTransform;
        m_rotationAngle     = transform.m_rotationAngle;
        m_rotationAxis      = transform.m_rotationAxis;
        m_position          = transform.m_position;
        m_scale             = transform.m_scale;

        return *this;
        // there may be more to copy, have to test.
    }

    /****************************************************************************//*!
     @brief    Retrieves the global rotation matrix of this object from the global
               transformation matrix.

     @warning  Take note that in most cases, unless being called after the Transform3D
               Runtime has been executed, this will be the position from the previous
               frame.

     @return   An oom::mat4 that represents the current rotation matrix
               of this GameObject in global coordinates.
    *//*****************************************************************************/
    oom::mat4 Transform3D::GetGlobalRotationMatrix() const
    {
        oom::mat4 result{ m_globalTransform };
        result[3] = oom::vec4{ 0.f, 0.f, 0.f, 1.0f };
        oom::vec3 scalar{ GetGlobalScale() };

        result[0] /= scalar[0];
        result[1] /= scalar[1];
        result[2] /= scalar[2];

        return result;
    }
    


    /****************************************************************************//*!
     @brief    Retrieves the global rotation of this object in degress
               from the global transformation matrix.
               link to explaination : https://tinyurl.com/DeriveTRSfrom2dMat

     @warning  Take note that in most cases, unless being called after the Transform
               Runtime has been executed, this will be the rotation from the previous
               frame.

     @return   A float in degrees that represents the rotation angle of the previous
               frame of this Component in global coordinates.
    *//*****************************************************************************/
    float Transform3D::GetGlobalRotationDeg() const
    {
        return  GetGlobalRotationRad() * 180.f / oom::pi<float>();
    }

    /****************************************************************************//*!
     @brief    Retrieves the global rotation of this object in radians
               from the global transformation matrix.
               link to explaination : https://tinyurl.com/DeriveTRSfrom2dMat

     @warning  Take note that in most cases, unless being called after the Transform
               Runtime has been executed, this will be the rotation from the previous
               frame.

     @return   A float in radians that represents the rotation angle of the previous
               frame of this Component in global coordinates.
    *//*****************************************************************************/
    float Transform3D::GetGlobalRotationRad() const
    {
        oom::mat4 rotMat = GetGlobalRotationMatrix();
        //this should be of atan2 of either -b/a or c/d,
        return std::atan2f(-rotMat[1][0], rotMat[0][0]);
    }

    /****************************************************************************//*!
     @brief    Retrieves the global scale of this object from the global
               transformation matrix.

     @warning  Take note that in most cases, unless being called after the Transform3D
               Runtime has been executed, this will be the position from the previous
               frame.

     @return   An oom::vec3 that represents the current scale of this GameObject in
               global coordinates.
    *//*****************************************************************************/
    oom::vec3 Transform3D::GetGlobalScale() const
    {
        //calculate global scale by calculating the length of each row which represents
        //the scale of that particular axis.
        return oom::vec3{ oom::length(m_globalTransform[0]), oom::length(m_globalTransform[1]), oom::length(m_globalTransform[2]) };
    }

    void Transform_GetLocalPosition(Entity instanceID, float* x, float* y, float* z)
    {
        GameObject obj{ instanceID };
        Transform3D& transform = obj.GetComponent<Transform3D>();
        oom::vec3 position = transform.GetPosition();
        *x = position.x;
        *y = position.y;
        *z = position.z;
    }

    void Transform_SetLocalPosition(Entity instanceID, float x, float y, float z)
    {
        GameObject obj{ instanceID };
        Transform3D& transform = obj.GetComponent<Transform3D>();
        transform.SetPosition({ x, y, z });
    }

    void Transform_GetLocalScale(Entity instanceID, float* x, float* y, float* z)
    {
        GameObject obj{ instanceID };
        oom::vec3 const& scale = obj.GetComponent<Transform3D>().GetScale();
        *x = scale.x;
        *y = scale.y;
        *z = scale.z;
    }

    void Transform_SetLocalScale(Entity instanceID, float x, float y, float z)
    {
        GameObject obj{ instanceID };
        obj.GetComponent<Transform3D>().SetScale({ x, y, z });
    }

} // namespace engine