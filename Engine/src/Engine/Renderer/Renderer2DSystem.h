#pragma once

#include "Engine/ECS/System.h"
#include "Engine/Renderer/OrthographicCamera.h"

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
    explicit Renderer2DSystem(ECS_Manager& _ECS_Manager) : System{ _ECS_Manager }, m_orthoCam{-1,1,-1,1} {};
    explicit Renderer2DSystem(ECS_Manager& _ECS_Manager, OrthographicCamera cam) : System{ _ECS_Manager }, m_orthoCam{ cam } {};

    void SetOrthographicCamera(OrthographicCamera cam) { m_orthoCam = cam; }

    /****************************************************************************//*!
         @brief    Function that defines the per-frame updates for the Runtime.
    *//*****************************************************************************/
    void Update() /*override*/;

private:

    OrthographicCamera m_orthoCam;
};

}// namespace engine
