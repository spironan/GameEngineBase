/*****************************************************************//**
\file       ECS_Utility.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui@digipen.edu
\date       29/6/2021
\brief  
A header providing important constants and user defined types
to be used by the ECS system

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "pch.h"
#include <bitset>

namespace engine
{
	using Entity = std::uint32_t;
	constexpr Entity MAX_ENTITY = 5000;

	using ComponentType = std::uint16_t;
	const ComponentType MAX_COMPONENTS = 128;
	using Signature = std::bitset<MAX_COMPONENTS>;

	class ECS_Manager;
}