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

		RTTR_ENABLE();

	protected:
		glm::mat4 m_projection = glm::mat4{ 1.0f };
	};
}// end namespace engine