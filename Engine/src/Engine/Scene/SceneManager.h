/*****************************************************************//**
\file       SceneManager.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui@digipen.edu
\date       15/7/2021
\brief  
Scene Manager manages creation and deletion of multiple scenes.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "Scene.h"
#include "Utility/Hash.h"
#include <vector>
namespace engine
{
	class SceneManager
	{
	public:
		using key_type = Scene::ID_type;
		using container_type = std::unordered_map<key_type,Scene>;
	private:
		SceneManager() = default;
		~SceneManager() = default;

		static SceneManager& GetInstance();

	private:
		container_type m_scenes{};
		key_type m_active_scene_id{};

	public:
		/*********************************************************************************//*!
		\brief    Adds a scene to the scene manager and if this is the first scene added
		or the only scene, sets this as the active scene
		 
		\param    filename 
			filename of the scene file this scene uses
		\return   
			the added scene
		*//**********************************************************************************/
		static Scene& AddScene(std::string filename, bool active = false);
		/*********************************************************************************//*!
		\brief    Same as AddScene but also loads the scene and set it as the active scene

		\param    filename 
			filename of the scene file this scene uses
		\return
			the added scene
		*//**********************************************************************************/
		static Scene& CreateScene(std::string filename);
		/*********************************************************************************//*!
		\brief    Returns the current active scene
		 
		\return   current active scene
		
		*//**********************************************************************************/
		static Scene& GetActiveScene();
		/*********************************************************************************//*!
		\brief    Gets the scene with the specified filename
		 
		\param    filename
			the specified filename
		\return   
			the scene with the specified filename, if not found, returns the first scene
		*//**********************************************************************************/
		static Scene& GetScene(key_type id);
		/*********************************************************************************//*!
		\brief    Sets the current active scene as the scene with the specified filename
		 
		\param    filename filename of the scene to set
		\return   current active scene after it has been set
		
		*//**********************************************************************************/
		static void SetActiveScene(key_type id);
		/*********************************************************************************//*!
		\brief    Gets the world of the current active scene
		 
		\return   world of the current active scene
		
		*//**********************************************************************************/
		static World& GetActiveWorld();
		/*********************************************************************************//*!
		\brief    Gets the root gameobject of the world of the current active scene
		 
		\return   root gameobject of the world of the current active scene
		
		*//**********************************************************************************/
		static GameObject& GetActiveRoot();
		/*********************************************************************************//*!
		\brief    Removes a scene from the scene manager, currently disabled
		 
		\param    filename filename of the scene to remove
		
		*//**********************************************************************************/
		/*static void RemoveScene(std::string filename)
		{
			for (iterator i = GetInstance().m_scenes.begin(); i != GetInstance().m_scenes.end(); ++i)
			{
				if (i->m_filename == filename)
				{
					GetInstance().m_scenes.erase(i);
					return;
				}
			}
		}*/
	};

}

