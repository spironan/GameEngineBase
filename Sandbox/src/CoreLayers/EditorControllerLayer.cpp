#include "EditorControllerLayer.h"

EditorControllerLayer::EditorControllerLayer()
    : engine::Layer("Editor Controller Layer")
{
}

void EditorControllerLayer::OnUpdate(engine::Timestep dt)
{
    //Works on whatever is the current layer
    // need some way to create a hidden "dont destroy on load" camera

    //Editor Scene-only input detection and resolution.
    // move camera here
    float deltaTime = static_cast<float>(dt);

    if (engine::Input::IsMouseButtonDown(ENGINE_MOUSE_BUTTON_RIGHT))
    {
        if (engine::Input::IsKeyDown(ENGINE_KEY_W))
        {
            m_editorCamera.Transform().Position().y += CameraMovingSpeed * deltaTime;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_S))
        {
            m_editorCamera.Transform().Position().y -= CameraMovingSpeed * deltaTime;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_D))
        {
            m_editorCamera.Transform().Position().x += CameraMovingSpeed * deltaTime;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_A))
        {
            m_editorCamera.Transform().Position().x -= CameraMovingSpeed * deltaTime;
        }
    }

}
