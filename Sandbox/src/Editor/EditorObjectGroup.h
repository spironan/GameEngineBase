/*****************************************************************//**
 * \file   EditorObjectGroup.h
 * \brief  Contains Static Vars and Functions used by
 *		   Hierarchy
 *		   Inspector
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "testclass.h"//remove this once ecs is done
#include "Engine/ECS/ECS_Utility.h"//entity

class ObjectGroup
{
public:
	static engine::Entity s_FocusedObject;

};

