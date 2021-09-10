#include "EditorFrameBufferView.h"
#include "Engine.h"

#include "../EditorObjectGroup.h"
#include <oom/oom.hpp>
#include <imgui.h>
#include <imgui_internal.h>
void FrameBuffer::Show()
{
	m_fb = engine::FramebufferPool::Get("2D_Framebuffer")->GetImage()->GetID();
	engine::World& activeWorld = engine::SceneManager::GetActiveWorld();
	//activeWorld.GetSystem<engine::Renderer2DSystem>()->SetCamera(DefaultCamera(), m_defaultCamera.Transform().GetGlobalPosition());
	activeWorld.GetSystem<engine::Renderer2DSystem>()->Update();



	engine::Window& x = engine::Application::Get().GetWindow();
	int width = x.GetSize().first;
	int height = x.GetSize().second;

	auto ar = (float)width / height;


	if (!ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing))
	{
		ImGui::End();
		return;
	}
	if (!ObjectGroup::s_FocusedObject)
	{
		ImGui::End();
		return;
	}

	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();
	vMin.x += ImGui::GetWindowPos().x;
	vMin.y += ImGui::GetWindowPos().y;
	vMax.x += ImGui::GetWindowPos().x;
	vMax.y += ImGui::GetWindowPos().y;

	ImVec2 vpDim = { vMax.x - vMin.x ,vMax.y - vMin.y };
	auto myW = vMax.x - vMin.x;
	auto myH = vMax.y - vMin.y;

	// opengl loves saving framebuffers upside down, 
	// so we flip the UVW in imgui::image()
	ImGui::Image((ImTextureID)static_cast<uint64_t>(m_fb), ImVec2{ ar * myH, myH }, { 0.0f,1.0f }, { 1.0f,0.0f });

	//Debug Red box
	//ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, ImU32(0xFF0000FF));

	ImGui::SetNextWindowSize({ vMax.x - vMin.x,vMax.y - vMin.y });
	ImGui::SetNextWindowPos(vMin);

	//window hole should be same size as content area
	ImGui::SetWindowHitTestHole(ImGui::GetCurrentWindow(), vMin, { myW,myH });

	// IMPORTANT: we now NEED to call this before begin frame
	ImGuizmo::SetRect(vMin.x, vMin.y, ar * myH, myH);
	ImGuizmo::BeginFrame();

	auto& transform = static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).GetComponent<engine::Transform3D>();
	oom::vec3 mScale = transform.GetGlobalScale();
	oom::vec3 mPosition = transform.GetGlobalPosition();
	oom::vec3 mRot{ 0,0,(transform.GetGlobalRotationRad()) };

	// Set the object TRS
	ImGuizmo::RecomposeMatrixFromComponents(oom::value_ptr(mPosition), oom::value_ptr(mRot), oom::value_ptr(mScale), oom::value_ptr(m_matrix));

	// Do your picking here. Then recompose matrix to update
	if (engine::Input::IsKeyPressed(engine::KeyCode::Q))
	{
		m_operation = ImGuizmo::TRANSLATE;
	}
	else if (engine::Input::IsKeyPressed(engine::KeyCode::W))
	{
		m_operation = ImGuizmo::ROTATE;
	}
	else if (engine::Input::IsKeyPressed(engine::KeyCode::E))
	{
		m_operation = ImGuizmo::SCALE;
	}

	auto& cam = DefaultCamera();
	auto& camTrans = m_defaultCamera.Transform();

	// TODO : way for camera to do view matrix
	auto& projection = cam.GetProjection();
	auto& view = cam.CalculateViewMatrix(camTrans.GetGlobalMatrix());

	auto mCurrentGizmoMode = ImGuizmo::WORLD;
	ImGuizmo::SetOrthographic(true);

	// supposed to call here but i am editing the interals to support docking
	//ImGuizmo::SetRect(vMin.x, vMin.y, myW, myH);
	ImGuizmo::SetDrawlist();
	if (ImGuizmo::Manipulate(oom::value_ptr(view),
		oom::value_ptr(projection),
		operation,
		mCurrentGizmoMode,
		oom::value_ptr(matrix),
		NULL,
		NULL))
	{
		// Return the matrix to its components so that we can update values
		ImGuizmo::DecomposeMatrixToComponents(oom::value_ptr(matrix),
			oom::value_ptr(mPosition),
			oom::value_ptr(mRot),
			oom::value_ptr(mScale));
		transform.SetPosition(mPosition);
		transform.SetScale(mScale);
		transform.SetRotationAngle(oom::degrees(mRot.z));
	}


	// transform.SetRotationAxis(mRot); // maybe??


	ImGui::End(); // End of Scene
}
