/************************************************************************************//*!
\file           GameSceneLayer.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Sept 06, 2021
\brief          The Main Layer that will run the actual game scene while editing.
                This means that certain systems dictated not to run during editing
                such as physics and scripting will not be updated here.
                Does not provide any functionality otherwise.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once
#include "UtilityLayers/SceneBaseLayer.h"

#include <string>

class GameSceneLayer : public SceneBaseLayer
{
private:
    std::string m_filepath; // filepath to scene

public:
    GameSceneLayer(std::string const& filepath);

    virtual void Init() override;

    virtual void Exit() override;

    virtual void OnUpdate(engine::Timestep dt) override;

    virtual void OnImGuiRender() override;

};