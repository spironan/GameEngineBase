/************************************************************************************//*!
\file          AssetManagementLayer.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          September 6, 2021
\brief         This file contains initialization of asset management system in the engine 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <Engine.h>

namespace engine
{

class AssetManagementLayer : public Layer{
public:
	AssetManagementLayer()
	{
		AssetManager::Init();
		using namespace engine;
		AssetManager::ImportAsset("Icons/Ouroboros_Folder.png");
		AssetManager::ImportAsset("Icons/Ouroboros_Folder.png");
		AssetManager::ImportAsset("Icons/Ouroboros_GenericFile.png");
		AssetManager::ImportAsset("Icons/Ouroboros_MP3.png");
		AssetManager::ImportAsset("Icons/Ouroboros_PNG.png");
		AssetManager::ImportAsset("Icons/Ouroboros_WAV.png");
		AssetManager::ImportAsset("Icons/Ouroboros_Prefab.png");
		AssetManager::ImportAsset("Icons/Ouroboros_Search_Icon_Black.png");
		AssetManager::ImportAsset("Icons/Ouroboros_Log_Icon_Black.png");
		AssetManager::ImportAsset("Icons/Ouroboros_Warning_Icon_Yellow.png");
		AssetManager::ImportAsset("Icons/Ouroboros_Error_Icon_Red.png");
	};

	virtual void OnDetach() override
	{
		AssetManager::Shutdown();
	}

};

}