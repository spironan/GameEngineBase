#include "pch.h"

#include "PrefabComponentSystem.h"

#include "Utility/Hash.h"
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
