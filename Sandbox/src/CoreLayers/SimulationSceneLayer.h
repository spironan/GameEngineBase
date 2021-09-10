/***********************************************************************************//*!
\file           SimulationLayer.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Sept 06, 2021
\brief          The Main Layer that will run the actual game scene while playing.
                This means that all systems that will run in the final binary 
                will be ran similarly here.
                Does not provide any functionality otherwise.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "UtilityLayers/SceneBaseLayer.h"

class SimulationSceneLayer : public SceneBaseLayer
{
public:
    SimulationSceneLayer();

    virtual void Init() override;
    virtual void Exit() override;
    virtual void OnUpdate(engine::Timestep dt) override;

    virtual void OnImGuiRender() override;

    void StartSimulation();

    void StopSimulation();
};