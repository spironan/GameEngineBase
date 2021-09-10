#include "EditorViewport.h"
#include "Engine.h"

#include "Editor/EditorObjectGroup.h"
#include "Engine/Renderer/EditorCamera.h"



void EditorViewport::Show()
{
	m_fb = engine::FramebufferPool::Get("2D_Framebuffer")->GetImage()->GetID();

	engine::Window& x = engine::Application::Get().GetWindow();
	int width = x.GetSize().first;
	int height = x.GetSize().second;

	auto ar = (float)width / height;


	if (!ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing ))
	{
		ImGui::End();
		return;
	}

	auto& transform = static_cast<engine::GameObject>(ObjectGroup::s_FocusedObject).GetComponent<engine::Transform3D>();

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
	ImGui::Image((ImTextureID)static_cast<uint64_t>(m_fb), {1600,900}, { 0.0f,1.0f }, { 1.0f,0.0f });

	//Debug Red box
	//ImGui::GetForegroundDrawList()->AddRect(vMin, {vMin.x + 1600, vMin.y+900}, ImU32(0xFF0000FF));

	ImGui::SetNextWindowSize({ myW,myH });
	ImGui::SetNextWindowPos(vMin);

	//window hole should be same size as content area
	ImGui::SetWindowHitTestHole(ImGui::GetCurrentWindow(), vMin, { myW,myH });

	// IMPORTANT: we now NEED to call this before begin frame
	ImGuizmo::SetRect(vMin.x, vMin.y, 1600, 900);
	ImGuizmo::BeginFrame();
	oom::vec3 mScale = transform.GetGlobalScale();
	oom::vec3 mPosition = transform.GetGlobalPosition();
	oom::vec3 mRot{ 0,0,(transform.GetGlobalRotationDeg()) };


	// Set the object TRS
	ImGuizmo::RecomposeMatrixFromComponents(oom::value_ptr(mPosition), oom::value_ptr(mRot), oom::value_ptr(mScale), oom::value_ptr(m_matrix));

	// Do your picking here. Then recompose matrix to update

	auto mCurrentGizmoMode = ImGuizmo::WORLD;
	ImGuizmo::SetOrthographic(true);

	
	//static float det{};
	//oom::mat4 iden = oom::rotation_x_matrix(oom::radians((det = 90.0f)));
	//ImGuizmo::DrawGrid(oom::value_ptr(engine::EditorCamera::g_editorCam->GetView()),
	//                   oom::value_ptr(engine::EditorCamera::g_editorCam->GetProjection()),
	//                   oom::value_ptr(iden),
	//                   1.0f);
	ImGuizmo::SetDrawlist();
	

	if (ImGuizmo::Manipulate(oom::value_ptr(engine::EditorCamera::g_editorCam->GetView()),
		oom::value_ptr(engine::EditorCamera::g_editorCam->GetProjection()),
		m_operation,
		mCurrentGizmoMode,
		oom::value_ptr(m_matrix),
		NULL,
		NULL))
	{
		// Return the matrix to its components so that we can update values
		ImGuizmo::DecomposeMatrixToComponents(oom::value_ptr(m_matrix),
			oom::value_ptr(mPosition),
			oom::value_ptr(mRot),
			oom::value_ptr(mScale));
		transform.SetPosition(mPosition);
		transform.SetScale(mScale);
		transform.SetRotationAngle((mRot.z));
	}
	else if (ImGui::IsMouseDown(ImGuiMouseButton_Left) == false)
	{
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
	}


	ImGui::End(); // End of Scene
}
