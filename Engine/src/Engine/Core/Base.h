/************************************************************************************//*!
\file           Base.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 24, 2021
\brief          Base class that will collate all the most important and basic files that
                all other files can include and gain access to within the engine.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Memory/MemoryCommon.h"
//#include "Engine/Core/PlatformDetection.h"

#include "Engine/Core/Log.h"
#include "Engine/Core/Assert.h"
// -- instrumentation
#include "Engine/Debug/Instrumentor.h"

/****************************************************************************//*!
 @brief     Wrapper for binding a particular function to an event
*//*****************************************************************************/
#define ENGINE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }