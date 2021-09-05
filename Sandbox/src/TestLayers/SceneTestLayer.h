/************************************************************************************//*!
\file       SceneTestLayer.h
\project    <PROJECT_NAME>
\author     Lim Guan Hui, l.guanhui , 390009020
\par        email: l.guanhui\@digipen.edu
\date       August 2, 2021
\brief      

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <Engine.h>
#include "Seralizer.h"
#include "UtilityLayers/SceneBaseLayer.h"
#include "Engine/Prefab/PrefabComponentSystem.h"
#include "Engine/Prefab/EditorComponentSystem.h"
struct A
{
    int i = 1;
};

struct B
{
    int i = 2;
};

struct C
{
    int i = 3;
};

class SceneTestLayer : public SceneBaseLayer
{
public:

    SceneTestLayer(std::string scene_path)
        : SceneBaseLayer{ "Scene Test Layer" }
        , m_scene_path(scene_path)
    {
    }

    void Init() final override
    {
        engine::GameObject testrootGO = RootGameObject();
        testrootGO.Name() = "Scene";
        std::vector<engine::GameObject> entities;
		GetWorld()->RegisterSystem<engine::PrefabComponentSystem>();
		GetWorld()->RegisterSystem<engine::EditorComponentSystem>();

        Serializer::LoadWorld(m_scene_path);
    }

    void Exit() final override
    {
        //Serializer::SaveWorld(m_scene_path);
    }

    /*~SceneTestLayer()
    {
    }*/

    virtual void OnUpdate(engine::Timestep dt) override
    {
        SceneBaseLayer::OnUpdate(dt);
    }

    virtual void OnImGuiRender() override
    {
    }

protected:
    std::string m_scene_path;
};
