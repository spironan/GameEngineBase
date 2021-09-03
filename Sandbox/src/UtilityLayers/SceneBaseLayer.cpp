/************************************************************************************//*!
\file       SceneBaseLayer.cpp
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
    m_scene.GetWorld().RegisterSystem<engine::TransformSystem>();
    Init();
}

void SceneBaseLayer::OnDetach() 
{
    //engine::SceneManager::SetActiveScene(m_scene.GetID()); -- guaranteed to be exiting correctly
    Exit();
    m_scene.Unload();
}

void SceneBaseLayer::Init() { };

void SceneBaseLayer::OnUpdateEnd(engine::Timestep dt)
{
	m_scene.GetWorld().ProcessDeletions();
}
void SceneBaseLayer::Exit() { };

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

