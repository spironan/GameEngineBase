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

		static SceneManager& GetInstance()
		{
			static SceneManager scene_manager;
			return scene_manager;
		}

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
		static Scene& AddScene(std::string filename, bool active = false)
		{
			auto result = GetInstance().m_scenes.emplace(std::make_pair(utility::StringHash::GenerateFNV1aHash(filename), filename));
			ENGINE_VERIFY(result.second);
			auto& scene = (*(result.first)).second;
			scene.SetID(result.first->first);
			if (GetInstance().m_scenes.size() == 1)
			{
				GetInstance().m_active_scene_id = scene.GetID();
			}


			return scene;
		}
		/*********************************************************************************//*!
		\brief    Same as AddScene but also loads the scene and set it as the active scene

		\param    filename 
			filename of the scene file this scene uses
		\return
			the added scene
		*//**********************************************************************************/
		static Scene& CreateScene(std::string filename)
		{
			auto& temp = GetInstance().AddScene(filename, true);
			temp.Load();

			return temp;
		}
		/*********************************************************************************//*!
		\brief    Returns the current active scene
		 
		\return   current active scene
		
		*//**********************************************************************************/
		static Scene& GetActiveScene()
		{
			ENGINE_VERIFY(GetInstance().m_scenes.find(GetInstance().m_active_scene_id) != GetInstance().m_scenes.end());
			return GetInstance().m_scenes.find(GetInstance().m_active_scene_id)->second;

		}
		/*********************************************************************************//*!
		\brief    Gets the scene with the specified filename
		 
		\param    filename
			the specified filename
		\return   
			the scene with the specified filename, if not found, returns the first scene
		*//**********************************************************************************/
		static Scene& GetScene(key_type id)
		{
			ENGINE_VERIFY(GetInstance().m_scenes.find(id) != GetInstance().m_scenes.end());
			return GetInstance().m_scenes.find(id)->second;

		}
		/*********************************************************************************//*!
		\brief    Sets the current active scene as the scene with the specified filename
		 
		\param    filename filename of the scene to set
		\return   current active scene after it has been set
		
		*//**********************************************************************************/
		static void SetActiveScene(key_type id)
		{
			ENGINE_VERIFY(GetInstance().m_scenes.find(id) != GetInstance().m_scenes.end());
			GetInstance().m_active_scene_id = id;
		}
		/*********************************************************************************//*!
		\brief    Gets the world of the current active scene
		 
		\return   world of the current active scene
		
		*//**********************************************************************************/
		static World& GetActiveWorld()
		{
			return GetActiveScene().GetWorld();

		}
		/*********************************************************************************//*!
		\brief    Gets the root gameobject of the world of the current active scene
		 
		\return   root gameobject of the world of the current active scene
		
		*//**********************************************************************************/
		static GameObject& GetActiveRoot()
		{
			return GetActiveScene().GetRoot();
		}
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

