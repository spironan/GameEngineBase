/************************************************************************************//*!
\file       SceneBaseLayer.cpp
\project    <PROJECT_NAME>
\author     Lim Guan Hui, l.guanhui , 390009020
\par        email: l.guanhui\@digipen.edu
\author     Chua Teck Lee, c.tecklee, 390008420
\par        email: c.tecklee\@digipen.edu
\date       Sept 5, 2021
\brief      A base scene to be inherited from to have convienience for ECS and 
            Gameobjects.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "SceneBaseLayer.h"


SceneBaseLayer::SceneBaseLayer(std::string scene_name)
    : Layer { scene_name }
    , m_scene { engine::SceneManager::CreateScene(scene_name) }
{
    m_scene.SetWorldAsActive();
}

SceneBaseLayer::~SceneBaseLayer()
{
    if(m_scene.IsLoaded()) m_scene.Unload();
}

engine::Scene::ID_type SceneBaseLayer::GetID() const
{
    return m_scene.GetID();
}

void SceneBaseLayer::OnAttach()
{
    engine::SceneManager::SetActiveScene(m_scene.GetID());
    m_scene.Load();
    m_world = &m_scene.GetWorld();
    m_world->RegisterSystem<engine::TransformSystem>();

    //Create Default Camera
    engine::Window& x = engine::Application::Get().GetWindow();
    auto [width, height] = x.GetSize();
    m_defaultCamera = CreateGameObject();
    m_defaultCamera.Name() = "Default Camera";
    auto& cam = m_defaultCamera.AddComponent<engine::SceneCamera>();
    cam.UpdateViewportSize(width, height);

    // Initialize Scene
    Init();
}

void SceneBaseLayer::OnDetach() 
{
    //engine::SceneManager::SetActiveScene(m_scene.GetID()); -- guaranteed to be exiting correctly
    Exit();
    m_scene.Unload();
    m_world = nullptr;
}

void SceneBaseLayer::OnUpdateEnd(engine::Timestep dt)
{
	m_scene.GetWorld().ProcessDeletions();
}

/*********************************************************************************//*!
\brief    Create a gameobject from the scene
     
\return   the created gameobject
*//**********************************************************************************/
engine::GameObject SceneBaseLayer::CreateGameObject()
{
    return m_scene.CreateGameObject();
}
/*********************************************************************************//*!
\brief    Get the root gameobject from the scene
     
\return   root gameobject from the scene
    
*//**********************************************************************************/
engine::GameObject SceneBaseLayer::RootGameObject()
{
    return m_scene.GetRoot();
}

engine::SceneCamera SceneBaseLayer::DefaultCamera()
{
    return m_defaultCamera.GetComponent<engine::SceneCamera>();
}
/*********************************************************************************//*!
\brief    Saves the scene to its scene file
     
    
*//**********************************************************************************/
void SceneBaseLayer::SaveScene()
{
    m_scene.SaveToFile();
}
/*********************************************************************************//*!
\brief    Saves the scene to a specified scene file
     
\param    filename 
    name of specified scene file
    
*//**********************************************************************************/
void SceneBaseLayer::SaveSceneToFile(std::string const& filename)
{
    m_scene.SaveToFileName(filename);
}
/*********************************************************************************//*!
\brief    Sets this layer's scene as the active scene
     
    
*//**********************************************************************************/
void SceneBaseLayer::SetSceneAsActive()
{
    engine::SceneManager::SetActiveScene(m_scene.GetID());
}

