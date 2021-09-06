/************************************************************************************//*!
\file           Engine.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 05, 2021
\brief          Serves as the simple and easy 
                single-include file that sandbox project(customer)
                will use to grab enable them to use features in of the engine.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Base.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/Layer.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Assert.h"

#include "Engine/Core/Timestep.h"

#include "Engine/Core/Input.h"

// -- ECS
#include "Engine/ECS/GameObject.h"
#include "Engine/ECS/ECS.h"
#include "Engine/Transform/Transform.h"
#include "Engine/PhysicsCollision/Physics.h"
#include "Engine/Scripting/ScriptSystem.h"

// Asset management
#include "Engine/Asset/AssetsManager.h"

//Scene
#include "Engine/Scene/SceneManager.h"

// -- Rendering
#include "Engine/Renderer/2DRendering.h"


// -- Physics
#include "Engine/PhysicsCollision/Physics.h";
