/************************************************************************************//*!
\file          PhysicsTestLayer.h
\project       <PROJECT_NAME>
\author        Chua Teck Lee, c.tecklee, 390008420
\par           email: c.tecklee\@digipen.edu
\date          July 27, 2021
\brief         Test Scene to test physics systems and components. 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "UtilityLayers/SceneBaseLayer.h"

class PhysicsTestLayer : public SceneBaseLayer
{
private:
    //engine::World& m_scene.GetWorld();
    engine::OrthographicCamera cam{ -1, 1, -1, 1 };
    int width{}, height{};
    engine::GameObject m_second, m_third;

    glm::vec2 upperbounds, lowerbounds;

public:

    PhysicsTestLayer();

    virtual void Init() override;

    virtual void OnUpdate(engine::Timestep dt) override;

    virtual void OnImGuiRender() override;

};
