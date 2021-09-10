#pragma once
#include <ImGuizmo.h>
#include <oom/oom.hpp>
#include "Engine.h"
class EditorViewport
{
public:
	EditorViewport() {};
	~EditorViewport() {};

	void Show();
private:
	oom::mat4 m_matrix{ 1.0f };

	ImGuizmo::OPERATION m_operation = ImGuizmo::TRANSLATE;

	engine::ooRendererID m_fb;
};

