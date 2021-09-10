#pragma once
#include "ImGuizmo.h"

class FrameBuffer
{
public:
	FrameBuffer() {};
	~FrameBuffer() {};

	void Show();
private:
	oom::mat4 m_matrix{ 1.0f };

	ImGuizmo::OPERATION m_operation = ImGuizmo::TRANSLATE;

	engine::ooRendererID m_fb;
};

