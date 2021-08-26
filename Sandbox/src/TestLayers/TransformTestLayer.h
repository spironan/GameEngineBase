/************************************************************************************//*!
\file           TransformTestLayer.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           June 22, 2021
\brief          Describes a Test scene used to test The Transform Components and Systems
                Functionality with ECS and Gameobjects.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "UtilityLayers/SceneBaseLayer.h"

/****************************************************************************//*!
 @brief     Describes a Test scene used to test The Transform Components 
            and Systems Functionality with ECS and Gameobjects.
*//*****************************************************************************/
class TransformTestLayer final : public SceneBaseLayer
{
private:
    std::vector<engine::GameObject> m_gos;
    std::vector<engine::GameObject>::iterator m_controller;
    std::vector<engine::GameObject>::iterator m_target;

    engine::GameObject m_camera;

    static constexpr float scaling = 50.f;

    static constexpr float MOVESPEED = 30.f;
    static constexpr float ROTATIONSPEED = 10.f;
    static constexpr float SCALINGSPEED = 20.f;

    void SelectNewTarget();

public:

    TransformTestLayer();

    void Init() final override;

    virtual void OnUpdate(engine::Timestep dt) override;

    virtual void OnImGuiRender() override;

};
