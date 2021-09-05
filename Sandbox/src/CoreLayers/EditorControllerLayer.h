#pragma once

#include <Engine.h>

class EditorControllerLayer final : public engine::Layer
{
private:
    engine::GameObject m_editorCamera;

    static constexpr float CameraMovingSpeed = 300.f;
public:
    EditorControllerLayer();

    virtual void OnUpdate(engine::Timestep dt) override;

    void SetEditorCamera(engine::GameObject editorCamera) { m_editorCamera = editorCamera; }
};