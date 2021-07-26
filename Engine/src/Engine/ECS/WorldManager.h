/*****************************************************************//**
\file       WorldManager.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui@digipen.edu
\date       14/7/2021
\brief  
World Manager manages the creation, access, and destruction of
multiple worlds.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "World.h"
namespace engine
{
	class WorldManager
	{
		using ID_type = World::ID_type;
		//using CallbackType = std::function<void(Entity)>;
	private:
		static WorldManager& GetInstance() 
		{
			static WorldManager wm;
			return wm;
		}

		static ID_type GenerateWorldID()
		{
			static ID_type id = 0;
			return id++;
		}

		static bool IsValid(ID_type id) { return (GetInstance().worlds.find(id) != GetInstance().worlds.end()); }

		std::unordered_map<ID_type,World> worlds;
		ID_type m_active_world = -1;
	public:

		static World& CreateWorld()
		{
			auto id = GenerateWorldID();
			auto& newWorld = GetInstance().worlds.emplace(id, World{}).first->second;
			newWorld.SetID(id);
			if (GetInstance().worlds.size() == 1)
				GetInstance().m_active_world = newWorld.GetID();
			return newWorld;
		};

		static World& GetWorld(ID_type id)
		{
			ENGINE_ASSERT(IsValid(id));
			return GetInstance().worlds[id];
		}

		static World& GetActiveWorld()
		{
			ENGINE_ASSERT(IsValid(GetInstance().m_active_world));
			return GetInstance().worlds[GetInstance().m_active_world];
		}

		static void SetActiveWorld(ID_type id)
		{
			ENGINE_ASSERT(IsValid(id));
			GetInstance().m_active_world = id;
		}

		static void DestroyWorld(ID_type id)
		{
			if (IsValid(id) == false)
				return;
			if (id == GetInstance().m_active_world)
				GetInstance().m_active_world = -1;
			GetInstance().worlds.erase(id);
			if (GetInstance().worlds.size() > 1)
				GetInstance().m_active_world = (*GetInstance().worlds.begin()).first;

		}

	};
}