/************************************************************************************//*!
\file           EditorControllerLayer.h
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