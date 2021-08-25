#include "pch.h"

#include "PrefabComponentSystem.h"
#include "PrefabComponent.h"
#include "Utility/Hash.h"
#include "Seralizer.h"
namespace engine
{
void PrefabComponentSystem::SavePrefab()
{

}

void PrefabComponentSystem::AddPrefab(const std::string& filepath)
{
    utility::StringHash::size_type fileHash = utility::StringHash::GenerateFNV1aHash(filepath);
	if (m_prefabDetails.find(fileHash) != m_prefabDetails.end())
		return;

	//use serializer to serialize prefab
	Entity headNode = Serializer::LoadObject(filepath, -1);//not shown in hierarchy
	ENGINE_ASSERT(headNode == 0);//means seralization failed

	static_cast<GameObject>(headNode).GetComponent<PrefabComponent>().m_RootNode = true;
	m_prefabDetails[fileHash] = FileDetails{ headNode,filepath };
}

Entity PrefabComponentSystem::GetPrefab(const std::string& filepath)
{
	utility::StringHash::size_type fileHash = utility::StringHash::GenerateFNV1aHash(filepath);
	auto& item =  m_prefabDetails.find(fileHash);
	if (item != m_prefabDetails.end())
		return 0;

	return item->second.head;
}

}
