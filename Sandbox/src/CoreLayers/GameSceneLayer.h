#pragma once
#include "UtilityLayers/SceneBaseLayer.h"

class GameSceneLayer : public SceneBaseLayer
{
public:
    GameSceneLayer();

    virtual void Init() override;

    virtual void OnUpdate(engine::Timestep dt) override;

    virtual void OnImGuiRender() override;
};