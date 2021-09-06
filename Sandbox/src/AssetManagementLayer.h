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

	virtual void OnAttach() override 
	{
		AssetManager::Init();
	}

};

}