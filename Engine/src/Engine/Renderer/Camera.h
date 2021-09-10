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

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <oom/oom.hpp>

#include "Engine/ECS/Component.h"

#include <rttr/type>

namespace engine
{
	class Camera
	{
	public:
		/**
		 * \brief Projection types of the scene camera.
		 *
		 */
		enum class ProjectionType
		{
			Perspective = 0, Orthographic = 1
		};
	public:
		/*-----------------------------------------------------------------------------*/
		/* Constructors and Destructors                                                */
		/*-----------------------------------------------------------------------------*/
		Camera() = default;
		Camera(Camera const&) = default;
		Camera(Camera&&) = default;
		Camera& operator=(Camera const&) = default;
		Camera& operator=(Camera&&) = default;

		Camera(const oom::mat4& projection) :m_projection{ projection } 
		{}

		/*-----------------------------------------------------------------------------*/
		/* Getter Functions                                                            */
		/*-----------------------------------------------------------------------------*/
		const oom::mat4& GetProjection() const { return m_projection; }

		/*-----------------------------------------------------------------------------*/
		/* Setter Functions                                                            */
		/*-----------------------------------------------------------------------------*/
		void SetProjectionMatrix(const oom::mat4& projectionMatrix) { m_projection = projectionMatrix; }

		oom::mat4 CalculateViewMatrix(const oom::mat4& position) { return oom::inverse(position).value(); }

		RTTR_ENABLE();

	protected:
		oom::mat4 m_projection = oom::mat4{ 1.0f };
	};
}// end namespace engine