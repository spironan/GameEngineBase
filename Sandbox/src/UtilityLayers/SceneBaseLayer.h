/************************************************************************************//*!
\file       SceneBaseLayer.h
\project    <PROJECT_NAME>
\author     Lim Guan Hui, l.guanhui , 390009020
\par        email: l.guanhui\@digipen.edu
\date       July 28, 2021
\brief      Base scene layer class

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <Engine.h>

/****************************************************************************//*!
 @brief     A base scene to be inherited from to have convienience for ECS and 
 Gameobjects.
*//*****************************************************************************/
class SceneBaseLayer : public engine::Layer
{
protected:
    engine::Scene& m_scene;

public:

    SceneBaseLayer(std::string scene_name)
        : Layer{ "SceneBaseLayer" }
        , m_scene{ engine::SceneManager::CreateScene(scene_name) }{
        m_scene.GetWorld().RegisterSystem<engine::TransformSystem>();
    
    }

    ~SceneBaseLayer()
    {
        engine::SceneManager::RemoveScene(m_scene.GetSceneName());
    }
    virtual void OnUpdate(engine::Timestep dt) override
    {
        m_scene.GetWorld().GetSystem<engine::TransformSystem>()->Update();
    }

    virtual void OnImGuiRender() override
    {
    }

protected:
    /*********************************************************************************//*!
    \brief    Create a gameobject from the scene
     
    \return   the created gameobject
    *//**********************************************************************************/
    virtual engine::GameObject CreateGameObject()
    {
        return m_scene.CreateGameObject();
    }
    /*********************************************************************************//*!
    \brief    Get the root gameobject from the scene
     
    \return   root gameobject from the scene
    
    *//**********************************************************************************/
    virtual engine::GameObject RootGameObject()
    {
        return m_scene.GetRoot();
    }
    /*********************************************************************************//*!
    \brief    Saves the scene to its scene file
     
    
    *//**********************************************************************************/
    virtual void SaveScene()
    {
        m_scene.SaveToFile();
    }
    /*********************************************************************************//*!
    \brief    Saves the scene to a specified scene file
     
    \param    filename 
        name of specified scene file
    
    *//**********************************************************************************/
    virtual void SaveSceneToFile(std::string const& filename)
    {
        m_scene.SaveToFileName(filename);
    }
};
