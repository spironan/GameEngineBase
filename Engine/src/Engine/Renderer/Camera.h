/************************************************************************************//*!
\file          Camera.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 30, 2021
\brief         This file abstracts a base camera object which holds basic knowledge of  projection.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/ECS/Component.h"

#include <rttr/type>

namespace engine
{
	class Camera
	{
	public:
		/*-----------------------------------------------------------------------------*/
		/* Constructors and Destructors                                                */
		/*-----------------------------------------------------------------------------*/
		Camera() = default;
		Camera(Camera const&) = default;
		Camera(Camera&&) = default;
		Camera& operator=(Camera const&) = default;
		Camera& operator=(Camera&&) = default;

		/*-----------------------------------------------------------------------------*/
		/* Getter Functions                                                            */
		/*-----------------------------------------------------------------------------*/
		const glm::mat4& GetProjection() const { return m_projection; }

		/*-----------------------------------------------------------------------------*/
		/* Setter Functions                                                            */
		/*-----------------------------------------------------------------------------*/
		void SetProjectionMatrix(const glm::mat4& projectionMatrix) { m_projection = projectionMatrix; }

		glm::mat4 CalculateViewMatrix(const glm::mat4& position) { return glm::inverse(position); }

		RTTR_ENABLE();

	protected:
		glm::mat4 m_projection = glm::mat4{ 1.0f };
	};
}// end namespace engine