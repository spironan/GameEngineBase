#pragma once
#include "EditorComponent.h"
#include <rttr/registration>

RTTR_REGISTRATION
{
	using namespace rttr;
registration::class_<EditorComponent>("Editor Component");
}
