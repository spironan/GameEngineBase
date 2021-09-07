/************************************************************************************//*!
\file          ColliderDebugDraw.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          September 8, 2021
\brief         This file facilitates the debug drawing of colliders. Can be stripped from final game. 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/ECS/Component.h"

#include "oom/oom.hpp"

#include <rttr/type>

namespace engine
{

class ColliderDebugDraw final : public Component
{
public:
	ColliderDebugDraw() = delete;
	ColliderDebugDraw(ColliderDebugDraw const&) = default;
	ColliderDebugDraw(ColliderDebugDraw&&) = default;
	ColliderDebugDraw& operator=(ColliderDebugDraw const&) = default;
	ColliderDebugDraw& operator=(ColliderDebugDraw&&) = default;
	virtual ~ColliderDebugDraw() override = default;

	ColliderDebugDraw(Entity _entityID, bool _active = true);
	//ColliderDebugDraw(const oom::vec4& colour = { 0.1f, 1.0f, 0.1f, 1.0f });

	 /*********************************************************************************//*!
		\brief    Gets the colour tint of the texture

		\return   Colour
		*//**********************************************************************************/
	const oom::vec4& GetColor() { return m_color; }

	/*-----------------------------------------------------------------------------*/
	/* Setter Functions                                                            */
	/*-----------------------------------------------------------------------------*/

	/*********************************************************************************//*!
	\brief    Sets the colour of the debug line

	\param    col tint to set
	*//**********************************************************************************/
	void SetColor(const oom::vec4& col) { m_color = col; }

	/*********************************************************************************//*!
	\brief    Sets the colour of the debug line

	\param    r red val
	\param    g green val
	\param    b blue val
	\param    alpha transparency value
	*//**********************************************************************************/
	void SetColor(float r, float g, float b, float alpha = 1.0f) { m_color.r = r; m_color.g = g; m_color.b = b; m_color.a = alpha; }

	RTTR_ENABLE();

private:
	oom::vec4 m_color{ 0.1f, 1.0f, 0.1f, 1.0f };
};
}
