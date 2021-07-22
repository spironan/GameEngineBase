#pragma once
#include <map>
#include <vector>
#include "Engine/Core/KeyCode.h"
class KeyBindingView
{
public:
	KeyBindingView();

	void Show(bool* active);
	void KeyboardWindow();
	void PreviewShortcuts();
private:
	void* img_id;//change this to whatever the format for id once the loading is done
	struct KeyData
	{
		float min_x;
		float min_y;
		float max_x;
		float max_y;
	};
	std::map < engine::Key, KeyData> m_keyboard_data;
	std::vector<engine::Key> m_keypressed;
	std::vector<engine::Key> m_keyhighlight;
};

