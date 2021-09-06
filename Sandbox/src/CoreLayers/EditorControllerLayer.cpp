/************************************************************************************//*!
\file           EditorControllerLayer.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Sept 06, 2021
\brief          The Editor Controller will latch on to the current scene and provide
                additional controls for the user to have the capabilities to
                manipulate objects with an editor-only camera and toolset.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
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

    /*if (engine::Input::IsMouseButtonDown(ENGINE_MOUSE_BUTTON_RIGHT))
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
    }*/

}
