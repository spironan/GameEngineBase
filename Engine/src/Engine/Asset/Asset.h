#pragma once

#include <string>

#include "AssetTypes.h"

namespace engine
{
	// Texture
	// Sound
	// Scene

	class Asset
	{
	public:

		virtual ~Asset(){}


	protected:
		std::string m_assetName;
		std::string m_fileName;
	};


}
