#pragma once
#include "PrefabView.h"
#include "imgui.h"


PrefabView::PrefabView() :m_active{true}
{
}

PrefabView::~PrefabView()
{
}

void PrefabView::Show()
{
	ImGui::Begin("PrefabEditor", &m_active);


	ImGui::End();
}
