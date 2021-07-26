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
	private:
		SceneManager() = default;
		~SceneManager() = default;

		static SceneManager& GetInstance()
		{
			static SceneManager scene_manager;
			return scene_manager;
		}

	private:
		std::list<Scene> m_scenes;
		std::list<Scene>::iterator active_scene = m_scenes.end();

	public:
		static Scene& CreateScene(std::string filename)
		{
			auto& scene = GetInstance().m_scenes.emplace_back(filename);
			if (GetInstance().m_scenes.size() == 1)
			{
				GetInstance().active_scene = GetInstance().m_scenes.begin();
			}
			return scene;
		}

		static Scene& AddScene(std::string filename)
		{
			auto& scene = GetInstance().m_scenes.emplace_back(filename);
			if (GetInstance().m_scenes.size() == 1)
			{
				GetInstance().active_scene = GetInstance().m_scenes.begin();
			}
			return scene;
		}

		static Scene& GetActiveScene()
		{
			ENGINE_ASSERT(GetInstance().m_scenes.empty());
			ENGINE_ASSERT(GetInstance().active_scene != GetInstance().m_scenes.end());
			return *GetInstance().active_scene;

		}

		static World& GetActiveWorld()
		{
			return GetActiveScene().GetWorld();

		}

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


//......



/*
* 
* 
auto& scene = SceneManager::CreateScene("Scene1.scn");

scene.Load();
scene.GetWorld()->RegisterSystem<A>();

world.System<A>.Init();

.....

world.System<A>.Update();

...

world.System<A>.Exit();


*/
