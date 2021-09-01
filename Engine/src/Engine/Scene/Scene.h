/*****************************************************************//**
\file       Scene.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui\@digipen.edu
\date       15/7/2021
\brief  
Scene is a class that can load a World from a scene file, thus
creating a World with information contained in the scene file, 
and also save a loaded world's information into a scene file.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "Engine/ECS/WorldManager.h"
#include "Engine/ECS/GameObject.h"
#include <string>
#include "Utility/Hash.h"
namespace engine
{
	class SceneManager;
	class Scene
	{
		friend class SceneManager;
	public:
		using ID_type = utility::StringHash::size_type;

	private:

		std::string m_filename{};
		World* m_world = nullptr;
		//GameObject m_root{};
		GameObject m_root{ Entity{0} };
		ID_type m_id{0};
		//SparseContainer<GameObject,MAX_ENTITY> gameobjects{};
		/*********************************************************************************//*!
		\brief    Sets this scene's id
		 
		\param    id this scene's id
		
		*//**********************************************************************************/
		void SetID(ID_type id);

	public:
		/*********************************************************************************//*!
		\brief    Scene must be constructed with a given name
		 
		\param    filename given name of scene
		
		*//**********************************************************************************/
		explicit Scene(std::string filename);
		~Scene();
		/*********************************************************************************//*!
		\brief    Returns the scene's name
		 
		\return   the scene's name
		
		*//**********************************************************************************/
		std::string GetSceneName();
		/*********************************************************************************//*!
		\brief    Loads a world into the scene
		 
		\return   reference to the loaded world
		
		*//**********************************************************************************/
		World& Load();
		/*********************************************************************************//*!
		\brief    True if scene has a world loaded, false otherwise
		 
		\return   True if scene has a world loaded, false otherwise
		
		*//**********************************************************************************/
		bool IsLoaded();
		/*********************************************************************************//*!
		\brief    Unloads a world from a scene. Calls SaveToFile() to save world to
		scene file
		 
		
		*//**********************************************************************************/
		void Unload();
		/*********************************************************************************//*!
		\brief    Saves a world in a scene into a scene file
		 
		
		*//**********************************************************************************/
		void SaveToFile();
		/*********************************************************************************//*!
		\brief    Saves a world in a scene into a specified scene file
		 
		\param    filename specified scene file's filename
		
		*//**********************************************************************************/
		void SaveToFileName(std::string const& filename);
		/*********************************************************************************//*!
		\brief    Get a reference to the loaded world
		 
		\return   reference to the loaded world
		
		*//**********************************************************************************/
		World& GetWorld();
		/*********************************************************************************//*!
		\brief    Returns reference to the root gameobject of the world loaded in the scene
		 
		\return   reference to the root gameobject of the world loaded in the scene
		
		*//**********************************************************************************/
		GameObject& GetRoot();
		/*********************************************************************************//*!
		\brief    Creates a gameobject that is parented to the root gameobject of the world 
		loaded in the scene
		 
		\return   created gameobject
		
		*//**********************************************************************************/
		GameObject CreateGameObject();
		/*********************************************************************************//*!
		\brief    Sets this world as the active world in the world manager
		 
		
		*//**********************************************************************************/
		void SetWorldAsActive();
		/*********************************************************************************//*!
		\brief    Get the id of this scene
		 
		\return   id of this scene
		
		*//**********************************************************************************/
		ID_type GetID();
	};
}