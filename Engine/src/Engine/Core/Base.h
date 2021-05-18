#pragma once

#include "Engine/Core/PlatformDetection.h"

#include "Engine/Core/Log.h"
#include "Engine/Core/Assert.h"
// -- instrumentation
#include "Engine/Debug/Instrumentor.h"

#define ENGINE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }