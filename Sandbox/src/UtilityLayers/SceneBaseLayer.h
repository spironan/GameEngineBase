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

    SceneBaseLayer(std::string scene_name);
    ~SceneBaseLayer();

    /*********************************************************************************//*!
    \brief    Get this scene's id
     
    \return   this scene's id
    
    *//**********************************************************************************/
    engine::Scene::ID_type GetID() const;

    void OnAttach() override;
    void OnDetach() override;

protected:
    
    virtual void Init();
	virtual void OnUpdateEnd(engine::Timestep dt) override;
    virtual void Exit();

    /*********************************************************************************//*!
    \brief    Create a gameobject from the scene
     
    \return   the created gameobject
    *//**********************************************************************************/
    virtual engine::GameObject CreateGameObject();
    /*********************************************************************************//*!
    \brief    Get the root gameobject from the scene
     
    \return   root gameobject from the scene
    
    *//**********************************************************************************/
    virtual engine::GameObject RootGameObject();
    /*********************************************************************************//*!
    \brief    Saves the scene to its scene file
     
    
    *//**********************************************************************************/
    virtual void SaveScene();
    /*********************************************************************************//*!
    \brief    Saves the scene to a specified scene file
     
    \param    filename 
        name of specified scene file
    
    *//**********************************************************************************/
    virtual void SaveSceneToFile(std::string const& filename);
    /*********************************************************************************//*!
    \brief    Sets this layer's scene as the active scene
     
    
    *//**********************************************************************************/
    virtual void SetSceneAsActive();
};
