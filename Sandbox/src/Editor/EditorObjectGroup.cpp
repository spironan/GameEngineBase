/*****************************************************************//**
 * \file   EditorObjectGroup.cpp
 * \brief  Contains Static vars and functions shared by hierarchy and
 *		   Inspector
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#include "EditorObjectGroup.h"

engine::Entity ObjectGroup::s_FocusedObject = 0;
engine::Entity ObjectGroup::s_DraggingObject = 0;
