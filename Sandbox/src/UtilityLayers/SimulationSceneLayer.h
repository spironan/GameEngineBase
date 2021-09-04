#pragma once

#include "SceneBaseLayer.h"

class SimulationSceneLayer : public SceneBaseLayer
{
public:
    SimulationSceneLayer();

    virtual void Init() override;

    virtual void OnUpdate(engine::Timestep dt) override;

    virtual void OnImGuiRender() override;
};