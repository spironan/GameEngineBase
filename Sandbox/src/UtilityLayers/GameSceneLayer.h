#pragma once

#include "SceneBaseLayer.h"

class GameSceneLayer : public SceneBaseLayer
{
private:
    engine::GameObject m_defaultCamera;

public:
    GameSceneLayer();

    virtual void Init() override;

    virtual void OnUpdate(engine::Timestep dt) override;

    virtual void OnImGuiRender() override;
};