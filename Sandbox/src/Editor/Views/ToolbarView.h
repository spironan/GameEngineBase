#pragma once
class ToolbarView
{
public:
	ToolbarView(float w = 30.0f, float h = 30.f) :btn_width(w),btn_height(h) {};
	void Show();
private:
	float btn_width;
	float btn_height;
	bool docking = false;
};

