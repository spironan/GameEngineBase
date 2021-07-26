#pragma once
#include "AssetTypes.h"
namespace engine{

class TextureLoader
{
public:
/*********************************************************************************//*! 
 * \brief Creates and loads a texture into gpu memory.
 * 
 * \param fileName path to the file
 * \return Texture asset created
*//**********************************************************************************/
	static Texture LoadFromFilePath(const std::string& fileName);

/*********************************************************************************//*!
 * \brief Releases the texture from gpu memory.
 *
 * \param ooTexID id of the texture
*//**********************************************************************************/
	static void DestroyTexture(ooTexID id); 

/*********************************************************************************//*!
 * \brief Releases the texture from gpu memory.
 *
 * \param Texture Texture to release
*//**********************************************************************************/
	static void DestroyTexture(Texture texture); 
};

}

