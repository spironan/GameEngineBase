#pragma once
#include<unordered_map>

#include "Utility/Hash.h"
#include "Engine/Asset/AssetMetadata.h"

namespace engine
{
	class AssetRegistry
	{
	public:
		AssetMetadata& operator[](utility::StringHash path);
		AssetMetadata& operator[](const  std::string& path);
		const AssetMetadata& Get(utility::StringHash path) const;

		void SetNamedHandle(const std::string& name, AssetHandle handle) { m_namedHandles[utility::StringHash(name)] = handle; }
		AssetHandle GetNamedHandle(utility::StringHash name) { return m_namedHandles[name]; }

		bool Contains(utility::StringHash path) const;
		size_t Remove(utility::StringHash path);
		void Clear();

		std::unordered_map<utility::StringHash::size_type, AssetMetadata>::iterator begin() { return m_AssetRegistry.begin(); }
		std::unordered_map<utility::StringHash::size_type, AssetMetadata>::iterator end() { return m_AssetRegistry.end(); }
		std::unordered_map<utility::StringHash::size_type, AssetMetadata>::const_iterator cbegin() { return m_AssetRegistry.cbegin(); }
		std::unordered_map<utility::StringHash::size_type, AssetMetadata>::const_iterator cend() { return m_AssetRegistry.cend(); }
	private:			   
		std::unordered_map<utility::StringHash::size_type, AssetMetadata> m_AssetRegistry;
		std::unordered_map<utility::StringHash::size_type, AssetHandle> m_namedHandles;
	};
}