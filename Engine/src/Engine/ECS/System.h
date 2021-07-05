/*****************************************************************//**
\file       System.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui@digipen.edu
\date       2/7/2021
\brief  
Base class to be inherited from when creating new systems 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include <set>
#include "ECS_Utility.h"
namespace engine
{
	class System
	{
	public:
		//additional helper functions to be included when requested
		virtual ~System() = default;
	private:

	};
}
