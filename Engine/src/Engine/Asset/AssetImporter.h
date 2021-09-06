/************************************************************************************//*!
\file          AssetImporter.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          September 6, 2021
\brief         This file contains implementation of an asset importer class which stores each type of serializer.
				The serializers are stored using unique_ptrs which handle their own memory for the lifetime of the program.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "AssetSerializer.h"

namespace engine
{

	class AssetImporter
	{
	public:
		static void Init();
		static void Serialize(const AssetMetadata& metadata, const std::shared_ptr<Asset>& asset);
		static void Serialize(const std::shared_ptr<Asset>& asset);
		static bool TryLoadData(const AssetMetadata& metadata, std::shared_ptr<Asset>& asset);

	private:
		static std::unordered_map<AssetType, std::unique_ptr<IAssetSerializer> > m_serializers;
	};

}