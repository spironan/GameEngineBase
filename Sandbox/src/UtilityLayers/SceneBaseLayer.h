/************************************************************************************//*!
\file       SceneBaseLayer.h
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
#pragma once

#include <Engine.h>

#define _EDITOR
#ifdef _EDITOR
#include "Engine/Renderer/EditorCamera.h"
#endif
class SceneBaseLayer : public engine::Layer
{
public:
    /*********************************************************************************//*!
    \brief    Default Constructor Creating a scene with the indicated scene name
              and setting its world to be the active one.
    *//**********************************************************************************/
    SceneBaseLayer(std::string scene_name, std::string const filepath = "");
    /*********************************************************************************//*!
    \brief    Default Destructor that unloads the scene if its loaded
    *//**********************************************************************************/
    ~SceneBaseLayer();

    /*********************************************************************************//*!
    \brief    Get this scene's id
     
    \return   this scene's id
    *//**********************************************************************************/
    engine::Scene::ID_type GetID() const;

    /*********************************************************************************//*!
    \brief    Behaviour that invokes when this layer is attached. Should not be
              overwritten by child class
    *//**********************************************************************************/
    void OnAttach() final override;
    /*********************************************************************************//*!
    \brief    Behaviour that invokes when this layer is detached. Should not be
              overwritten by child class
    *//**********************************************************************************/
    void OnDetach() final override;

protected:

    /*********************************************************************************//*!
    \brief    Processes all deletion. cannot be overriden by child class
    *//**********************************************************************************/
    void OnUpdateEnd(engine::Timestep dt) final override;

    /*********************************************************************************//*!
    \brief    Initialize Function that should be overloaded by child class for whatever
              additional initialization required
    *//**********************************************************************************/
    virtual void Init();
    /*********************************************************************************//*!
    \brief    Exit Function that should be overloaded by child class for whatever
              cleanup that may be required
    *//**********************************************************************************/
    virtual void Exit() {};
    /*********************************************************************************//*!
    \brief    Retrieve the current scene

    \return   the current scene
    *//**********************************************************************************/
    engine::Scene const& GetScene() const { return m_scene; }
    /*********************************************************************************//*!
    \brief    Retrieve the current world

    \return   the current world. nullptr if current scene is not active.
    *//**********************************************************************************/
    engine::World* GetWorld() const { return m_world; }
    /*********************************************************************************//*!
    \brief    Create a gameobject from the scene
     
    \return   the created gameobject
    *//**********************************************************************************/
    engine::GameObject CreateGameObject();
    /*********************************************************************************//*!
    \brief    Get the root gameobject from the scene
     
    \return   root gameobject from the scene
    *//**********************************************************************************/
    engine::GameObject RootGameObject();
    /*********************************************************************************//*!
    \brief    Get the default camera from the scene

    \return   the default camera of the scene
    *//**********************************************************************************/
    //engine::SceneCamera DefaultCamera();
    engine::EditorCamera& DefaultCamera();
    
    /*********************************************************************************//*!
    \brief    Saves the scene to its scene file
    *//**********************************************************************************/
    void SaveScene();
    /*********************************************************************************//*!
    \brief    Saves the scene to a specified scene file
     
    \param    filename 
        name of specified scene file
    *//**********************************************************************************/
    void SaveSceneToFile(std::string const& filename);
    /*********************************************************************************//*!
    \brief    Sets this layer's scene as the active scene
    *//**********************************************************************************/
    void SetSceneAsActive();

protected:
    engine::GameObject m_defaultCamera;

//private:
    engine::Scene& m_scene;
    engine::World* m_world = nullptr;
};
