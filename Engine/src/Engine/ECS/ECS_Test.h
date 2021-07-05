/*****************************************************************//**
\file       ECS_Test.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui@digipen.edu
\date       2/7/2021
\brief  
This file contains some tests for the ECS system being implemented

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "ECS.h"
namespace engine
{
	class Test_Transform final
	{
	public:
		Test_Transform(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
		float x, y, z;
	};

	struct Test_Health final 
	{
	public:
		int hp;
	};

	class Test_Transform_System final : public System
	{
	public:
		Test_Transform_System(ECS_Manager& _ECS_Manager) : m_ECS_Manager(_ECS_Manager) {};
		void Run()
		{
			auto view = m_ECS_Manager.GetComponentView<Test_Transform>();
			for (auto it : view)
			{
				auto transform = m_ECS_Manager.GetComponent<Test_Transform>(it);
				transform.x += 1.f;
			}
			LOG_TRACE("success");
		}

	private:
		ECS_Manager& m_ECS_Manager;
	};

	void ECS_Test()
	{
		LOG_TRACE("ECS_Test");
		//initialise ecs manager
		engine::ECS_Manager _ECS_Manager;
		_ECS_Manager.Init();

		//test create entity
		Entity temp_ent = _ECS_Manager.CreateEntity();
		//test register component
		_ECS_Manager.RegisterComponent<Test_Transform>();
		_ECS_Manager.RegisterComponent<Test_Health>();

		//test add component
		{
			_ECS_Manager.AddComponent(temp_ent, Test_Transform{ 1.0f,2.0f,3.0f });
			LOG_TRACE("test add component ");
			LOG_TRACE(_ECS_Manager.GetComponent<Test_Transform>(temp_ent).z == 3.f ? "success" : "failure");

			_ECS_Manager.AddComponent(temp_ent, Test_Health{ 25 });
			LOG_TRACE(_ECS_Manager.GetComponent<Test_Health>(temp_ent).hp == 25 ? "success" : "failure");
		}
		//test remove component
		{
			_ECS_Manager.RemoveComponent<Test_Transform>(temp_ent);
			LOG_TRACE("test remove component ");
			LOG_TRACE(_ECS_Manager.HasComponent<Test_Transform>(temp_ent) == false ? "success" : "failure");
		}

		//test emplace component
		{
			_ECS_Manager.EmplaceComponent<Test_Transform>(temp_ent, 4.0f, 5.0f, 6.0f);
			LOG_TRACE("test emplace component ");
			LOG_TRACE(_ECS_Manager.GetComponent<Test_Transform>(temp_ent).z == 6.0f ? "success" : "failure");
		}

		//test remove entity
		{
			_ECS_Manager.DestroyEntity(temp_ent);
			Test_Transform* tc = _ECS_Manager.TryGetComponent<Test_Transform>(temp_ent);
			LOG_TRACE("test remove entity");
			LOG_TRACE(tc ? "failure" : "success");
		}
		//test bulk entity creation , component addition, component removal
		{
			LOG_TRACE("test bulk entity creation , component addition, component removal");
			int num_times = 100;
			std::vector<Entity> entityList;
			for (int i = 0; i < num_times; i++)
			{
				entityList.emplace_back(_ECS_Manager.CreateEntity());
			}
			for (int i = 0; i < num_times; i++)
			{
				_ECS_Manager.EmplaceComponent<Test_Transform>(entityList[i], i * 2.f, i * 4.f, i * 6.f);
				_ECS_Manager.EmplaceComponent<Test_Health>(entityList[i], Test_Health{ i });
			}
			for (int i = 0; i < num_times; i++)
			{
				_ECS_Manager.RemoveComponent<Test_Transform>(entityList[i]);
				_ECS_Manager.RemoveComponent<Test_Health>(entityList[i]);
			}
			for (int i = 0; i < num_times; i++)
			{
				_ECS_Manager.DestroyEntity(entityList[i]);
			}
			LOG_TRACE("success");
			LOG_TRACE("finished test bulk entity creation , component addition, component removal");
		}
		//test transfer component
		{
			LOG_TRACE("test transfer component");
			Entity src = _ECS_Manager.CreateEntity();
			_ECS_Manager.EmplaceComponent<Test_Transform>(src, 2.f, 4.f, 8.f);
			Entity dst = _ECS_Manager.CreateEntity();
			_ECS_Manager.EmplaceComponent<Test_Health>(dst, Test_Health{ 55 });

			_ECS_Manager.TransferComponent<Test_Transform>(src, dst);
			LOG_TRACE((_ECS_Manager.HasComponent< Test_Transform>(src) == false &&
				_ECS_Manager.HasComponent< Test_Transform>(dst) == true) ? "success" : "failure");

			_ECS_Manager.TransferComponent<Test_Health>(dst, src);
			LOG_TRACE((_ECS_Manager.HasComponent<Test_Health>(dst) == false &&
				_ECS_Manager.HasComponent<Test_Health>(src) == true) ? "success" : "failure");
			_ECS_Manager.DestroyEntity(src);
			_ECS_Manager.DestroyEntity(dst);
		}
		//test copy component
		{
			LOG_TRACE("test copy component");
			Entity src = _ECS_Manager.CreateEntity();
			_ECS_Manager.EmplaceComponent<Test_Transform>(src, 2.f, 4.f, 8.f);
			Entity dst = _ECS_Manager.CreateEntity();
			_ECS_Manager.EmplaceComponent<Test_Health>(dst, Test_Health{ 55 });

			_ECS_Manager.CopyComponent<Test_Transform>(src, dst);
			LOG_TRACE(_ECS_Manager.HasComponent<Test_Transform>(dst) ? "success" : "failure");

			_ECS_Manager.CopyComponent<Test_Health>(dst, src);
			LOG_TRACE(_ECS_Manager.HasComponent<Test_Health>(src) ? "success" : "failure");

			_ECS_Manager.DestroyEntity(src);
			_ECS_Manager.DestroyEntity(dst);
		}

		//test view
		{
			LOG_TRACE("test view");
			int num_entt_test = 0;
			int num_entt_test_2 = 0;
			for (int i = 0; i < 50; i++)
			{
				Entity ent = _ECS_Manager.CreateEntity();
				if (i % 2 > 0)
				{
					_ECS_Manager.EmplaceComponent<Test_Transform>(ent, 2.f, 4.f, 8.f);
				}
				else
				{
					_ECS_Manager.EmplaceComponent<Test_Health>(ent, Test_Health{ 55 });
					_ECS_Manager.EmplaceComponent<Test_Transform>(ent, 2.f, 4.f, 8.f);
					++num_entt_test;
				}
				++num_entt_test_2;
			}

			auto view = _ECS_Manager.GetComponentView<Test_Transform, Test_Health>();
			for (auto it : view)
			{
				auto transform = _ECS_Manager.GetComponent<Test_Transform>(it);
				auto health = _ECS_Manager.GetComponent< Test_Health>(it);
				if ((transform.x == 2.f && health.hp == 55))
					--num_entt_test;
			}

			auto view2 = _ECS_Manager.GetComponentView<Test_Transform>();
			for (auto it : view2)
			{
				auto transform = _ECS_Manager.GetComponent<Test_Transform>(it);
				if ((transform.x == 2.f))
					--num_entt_test_2;
			}

			LOG_TRACE((num_entt_test == 0 && num_entt_test_2 == 0) ? "success" : "failure");
		}

		//test system
		{
			LOG_TRACE("test system");
			auto test_transform_system = _ECS_Manager.RegisterSystem<Test_Transform_System>(_ECS_Manager);
			test_transform_system->Run();
		}
	}
}