#include "KeyBindingView.h"
#include <imgui.h>
KeyBindingView::KeyBindingView()
{
	img_id = ImGui::GetIO().Fonts->TexID;
	//first row of the keyboard
	m_keyboard_data[engine::Key::GRAVE] = KeyData{ 0,0,100,100 };
	m_keyboard_data[engine::Key::D1] = KeyData{ 100,0,200,100 };
	m_keyboard_data[engine::Key::D2] = KeyData{ 200,0,300,100 };
	m_keyboard_data[engine::Key::D3] = KeyData{ 300,0,400,100 };
	m_keyboard_data[engine::Key::D4] = KeyData{ 400,0,500,100 };
	m_keyboard_data[engine::Key::D5] = KeyData{ 500,0,600,100 };
	m_keyboard_data[engine::Key::D6] = KeyData{ 600,0,700,100 };
	m_keyboard_data[engine::Key::D7] = KeyData{ 700,0,800,100 };
	m_keyboard_data[engine::Key::D8] = KeyData{ 800,0,900,100 };
	m_keyboard_data[engine::Key::D9] = KeyData{ 900,0,1000,100 };
	m_keyboard_data[engine::Key::D0] = KeyData{ 1000,0,1100,100 };
	m_keyboard_data[engine::Key::MINUS] = KeyData{ 1100,0,1200,100 };
	m_keyboard_data[engine::Key::EQUALS] = KeyData{ 1200,0,1300,100 };
	m_keyboard_data[engine::Key::BACKSPACE] = KeyData{ 1300,0,1500,100 };

}

void KeyBindingView::Show(bool* active)
{
	
	ImGui::SetNextWindowSize({1600,800});
	ImGui::Begin("Key Shortcuts",active,ImGuiWindowFlags_NoDocking);
	KeyboardWindow();
	PreviewShortcuts();
	ImGui::End();
}
void KeyBindingView::KeyboardWindow()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::BeginChild("Keyboard Preview", {0,400},true,ImGuiWindowFlags_AlwaysAutoResize);
	ImVec2 p = ImGui::GetCursorScreenPos();
	for (auto& key : m_keyboard_data)
	{
		ImGui::GetWindowDrawList()->AddImage(img_id,
											 { p.x + key.second.min_x ,p.y + key.second.min_y },
											 { p.x + key.second.max_x, p.y + key.second.max_y },
											 { 0,0 }, { 1,1 }, ImColor(255, 1, 0, 255));

	}
	for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
	{
		if (ImGui::IsKeyDown(i))
			m_keypressed.emplace_back((engine::KeyCode)i);
	}
	for (const auto& kp : m_keypressed)
	{
		KeyData& kd = m_keyboard_data[kp];
		ImGui::GetWindowDrawList()->AddImage(img_id,
											 { p.x + kd.min_x ,p.y + kd.min_y },
											 { p.x + kd.max_x, p.y + kd.max_y },
											 { 0,0 }, { 0.1f,1 }, ImColor(255, 255, 0, 255));
	}
	m_keypressed.clear();
	for (const auto& kp : m_keyhighlight)
	{
		KeyData& kd = m_keyboard_data[kp];
		ImGui::GetWindowDrawList()->AddImage(img_id,
											 { p.x + kd.min_x ,p.y + kd.min_y },
											 { p.x + kd.max_x, p.y + kd.max_y },
											 { 0,0 }, { 0.1f,1 }, ImColor(0, 255, 0, 255));
	}

	ImGui::EndChild();
}
void KeyBindingView::PreviewShortcuts()
{
	ImGui::BeginChild("Shortcuts", { 0,350 }, true);
	if (ImGui::Button("Show Hierarchy", {100,100}))
	{
		m_keyhighlight.clear();
		m_keyhighlight.emplace_back(engine::Key::D2);
	}
	ImGui::EndChild();
}