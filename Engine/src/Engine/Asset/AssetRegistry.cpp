#include "pch.h"
#include "AssetRegistry.h"
#include "Engine/Core/Log.h"

namespace engine
{

AssetMetadata&  AssetRegistry::operator[](utility::StringHash path)
{
    return m_AssetRegistry[path];
}

AssetMetadata& AssetRegistry::operator[](const std::string& path)
{
    return m_AssetRegistry[utility::StringHash(path)];
}

const AssetMetadata&  AssetRegistry::Get(utility::StringHash path) const
{
    return m_AssetRegistry.at(path);
   
}

bool  AssetRegistry::Contains(utility::StringHash path) const
{
    return (m_AssetRegistry.find(path) != m_AssetRegistry.end());
}

size_t  AssetRegistry::Remove(utility::StringHash path)
{
    return m_AssetRegistry.erase(path);
}

void  AssetRegistry::Clear()
{
    LOG_ENGINE_INFO("[ASSET] Clearing registry");
    m_AssetRegistry.clear();
}

}