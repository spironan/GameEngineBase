/*****************************************************************//**
\file       World.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui@digipen.edu
\date       14/7/2021
\brief  
World basically contains an ECS system inside it. It contains
entites, which have components, and systems which operate
on all entities that have specified components.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "ECS_Manager.h"

namespace engine
{
	class World : public ECS_Manager
	{
		friend class WorldManager;
	public:
		using ID_type = int;
		ID_type GetID()
		{
			return m_id;
		}
		
		/*GameObject& CreateGameObject() {

		};*/


		World() { Init(); };
	private:
		//explicit World(ID_type id) : m_id{ id } {};
		void SetID(ID_type id)
		{
			m_id = id;
		}
		
		ID_type m_id = 0;

		//GameObject m_root;
	};

}