/************************************************************************************//*!
\file       SceneTestLayer.h
\project    <PROJECT_NAME>
\author     Lim Guan Hui, l.guanhui , 390009020
\par        email: l.guanhui\@digipen.edu
\date       August 2, 2021
\brief      

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <Engine.h>
#include "UtilityLayers/SceneBaseLayer.h"

class SceneTestLayer : public SceneBaseLayer
{
public:

    SceneTestLayer(std::string scene_name)
        : SceneBaseLayer{ "SceneBaseLayer" }
    {
        engine::GameObject testGO = CreateGameObject();
        engine::GameObject testrootGO = RootGameObject();
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        SceneBaseLayer::OnUpdate(dt);
    }

    virtual void OnImGuiRender() override
    {
    }

protected:
   
};
