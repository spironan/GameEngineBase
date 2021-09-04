#pragma once

#include "SceneBaseLayer.h"

class EditorSceneLayer : public SceneBaseLayer
{
private:
    engine::GameObject m_defaultCamera;
    engine::GameObject m_editorCamera;


    static constexpr float CameraMovingSpeed = 300.f;
public:
    EditorSceneLayer();

    virtual void Init() override;

    virtual void OnUpdate(engine::Timestep dt) override;

    virtual void OnImGuiRender() override;
};