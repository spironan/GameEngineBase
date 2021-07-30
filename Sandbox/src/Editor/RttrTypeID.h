#pragma once
#include <vector>
#include <rttr/type.h>
#include "Engine/ECS/ECS_Utility.h"
#include "glm/glm.hpp"

namespace rttr_type_ID
{
	enum type_rttrID : int
	{
		type_INT,
		type_FLOAT,
		type_STRING,
		type_BOOL,
		type_ENTITY,
		type_VEC3,
		type_MAT4,
	};

	static std::vector <rttr::type::type_id> m_tracked_ids
	{
		rttr::type::get<int>().get_id(),
		rttr::type::get<float>().get_id(),
		rttr::type::get<std::string>().get_id(),
		rttr::type::get<bool>().get_id(),
		rttr::type::get<engine::Entity>().get_id(),
		rttr::type::get<glm::vec3>().get_id(),
		rttr::type::get<glm::mat4>().get_id()

	};
}


