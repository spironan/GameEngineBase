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
#include <vector>
namespace engine
{
	class SceneManager
	{
	public:
		using container_type = std::list<Scene>;
		using iterator = container_type::iterator;
		
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
		iterator active_scene = m_scenes.end();

	public:
		/*********************************************************************************//*!
		\brief    Adds a scene to the scene manager and if this is the first scene added
		or the only scene, sets this as the active scene
		 
		\param    filename 
			filename of the scene file this scene uses
		\return   
			the added scene
		*//**********************************************************************************/
		static Scene& AddScene(std::string filename)
		{
			auto& scene = GetInstance().m_scenes.emplace_back(filename);
			if (GetInstance().m_scenes.size() == 1)
			{
				GetInstance().active_scene = GetInstance().m_scenes.begin();
			}
			return scene;
		}
		/*********************************************************************************//*!
		\brief    Same as AddScene but also loads the scene

		\param    filename 
			filename of the scene file this scene uses
		\return
			the added scene
		*//**********************************************************************************/
		static Scene& CreateScene(std::string filename)
		{
			auto& temp = GetInstance().AddScene(filename);
			temp.Load();
			return temp;
		}
		/*********************************************************************************//*!
		\brief    Returns the current active scene
		 
		\return   current active scene
		
		*//**********************************************************************************/
		static Scene& GetActiveScene()
		{
			ENGINE_ASSERT(GetInstance().m_scenes.empty());
			ENGINE_ASSERT(GetInstance().active_scene != GetInstance().m_scenes.end());
			return *GetInstance().active_scene;

		}
		/*********************************************************************************//*!
		\brief    Gets the scene with the specified filename
		 
		\param    filename
			the specified filename
		\return   
			the scene with the specified filename, if not found, returns the first scene
		*//**********************************************************************************/
		static Scene& GetScene(std::string filename)
		{
			ENGINE_ASSERT(GetInstance().m_scenes.empty());
			for (auto& i : GetInstance().m_scenes)
			{
				if (i.m_filename == filename)
					return i;
			}
			return *GetInstance().m_scenes.begin();

		}
		/*********************************************************************************//*!
		\brief    Sets the current active scene as the scene with the specified filename
		 
		\param    filename filename of the scene to set
		\return   current active scene after it has been set
		
		*//**********************************************************************************/
		static Scene& SetActiveScene(std::string filename)
		{
			for (iterator i = GetInstance().m_scenes.begin(); i != GetInstance().m_scenes.end(); ++i)
			{
				if (i->m_filename == filename)
				{
					GetInstance().active_scene = i;
				}
			}
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

		/*static Scene& RemoveScene(std::string filename)
		{
			for ()
		}*/
	};

}

