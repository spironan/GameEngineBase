/************************************************************************************//*!
\file          Renderer2DSystem.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         File contains an API for rendering 2D sprite to 
                screen in batch using the ECS system 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/ECS/System.h"
#include "Engine/Transform/Transform3D.h"
#include "Engine/Renderer/Camera.h"

namespace engine
{
    //forward declaration
    class Sprite2D;

class Renderer2DSystem final : public System
{
  

public:
    /*-----------------------------------------------------------------------------*/
    /* Class Specific Type Aliases                                                 */
    /*-----------------------------------------------------------------------------*/
    using value_type = Sprite2D;
    using value_reference = value_type&;
    using value_const_reference = value_type const&;

    using container_type = std::vector<value_type>;
    using container_reference = container_type&;
    using container_iterator = container_type::iterator;
    using container_const_iterator = container_type::const_iterator;

    /*-----------------------------------------------------------------------------*/
    /* Explicit Constructor                                                        */
    /*-----------------------------------------------------------------------------*/
    explicit Renderer2DSystem(ECS_Manager& _ECS_Manager) : System{ _ECS_Manager }, m_projection{glm::ortho(-1,1,-1,1)} {};
    //explicit Renderer2DSystem(ECS_Manager& _ECS_Manager,const OrthographicCamera& cam) : System{ _ECS_Manager }, m_orthoCam{ cam } {};
    explicit Renderer2DSystem(ECS_Manager& _ECS_Manager, const Camera& cam) : System{ _ECS_Manager }, m_cam{ &cam } {SetCamera(cam, {0,0,0}); };

    /*********************************************************************************//*!
    \brief    Updates the rendering system to use the specified camera as the viewpoint
     
    \param    cam Copy of the camera
    
    *//**********************************************************************************/
    void SetCamera(const Camera& cam,const glm::vec3& position);

    /*********************************************************************************//*!
    \brief    Updates the rendering system to display to screen
     
    
    *//**********************************************************************************/
    void Update() /*override*/;

private:

    const Camera* m_cam;
    const Transform3D* m_transform;
    glm::mat4 m_projection;
    glm::mat4 m_view;
    glm::mat4 m_viewProj;
};

}// namespace engine
