/*****************************************************************//**
\file       Scene.cpp
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
#include "pch.h"
#include "Scene.h"
namespace engine
{
void Scene::SetID(ID_type id)
{
	m_id = id;
}


Scene::Scene(std::string filename) : m_filename{ std::move(filename) }
{ 

};
Scene::~Scene() { if (IsLoaded()) Unload(); }
		
std::string Scene::GetSceneName()
{
	return m_filename.substr(0, m_filename.find_first_of('.'));
}

World& Scene::Load()
{
	m_world = &WorldManager::CreateWorld();
	WorldManager::SetActiveWorld(m_world->GetID());
	m_root = GameObject{GameObject::Create{}};	//instantiate root game object
	//deserialise scene file and load objects here

	return *m_world;
}

bool Scene::IsLoaded()
{
	return m_world != nullptr;
}

void Scene::Unload()
{
	SaveToFile();

	if (m_world)
		WorldManager::DestroyWorld(m_world->GetID());
	m_world = nullptr;
}

void Scene::SaveToFile()
{
	SaveToFileName(m_filename);
}

void Scene::SaveToFileName(std::string const& filename)
{
	//save data to file here
}

World& Scene::GetWorld()
{
	ENGINE_ASSERT(IsLoaded());
	return *m_world;
}

GameObject& Scene::GetRoot()
{
	ENGINE_ASSERT(IsLoaded());
	return m_root;
}

GameObject Scene::CreateGameObject()
{
	ENGINE_ASSERT(IsLoaded());
	GameObject temp{ engine::GameObject::Create{} };
	m_root.AddChild(temp);
	return temp;
}

void Scene::SetWorldAsActive()
{
	WorldManager::SetActiveWorld(m_world->GetID());
}

Scene::ID_type Scene::GetID()
{
	return m_id;
}
}