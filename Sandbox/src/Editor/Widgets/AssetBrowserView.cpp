#include "AssetBrowserView.h"
#include <imgui.h>
#include "Engine/Asset/AssetsManager.h"
#include "Engine.h"
bool AssetBrowserView::s_showView = false;

AssetBrowserView::AssetReferencePoint* AssetBrowserView::s_assetReference = nullptr;
void AssetBrowserView::Show()
{
	static constexpr float imgSize = 50;
	static constexpr float padding = 15;
	if (s_showView == false)
		return;
	ImGui::SetNextWindowSize({ 600,600 });
	if(ImGui::Begin("Asset Browser", &s_showView) == false)
	{
		ImGui::End();
		return;
	}

	//gets a list of assets
	//if the asset is selected set s_assetReference.instance to the id
	auto textureList = engine::AssetManager::GetAll<engine::Texture>();
	float row = ImGui::GetContentRegionAvailWidth() / (padding + imgSize);
	ImGui::BeginTable("AssetTable", static_cast<int>(row));

	for (auto tex : textureList)
	{
		ImGui::TableNextColumn();
		ImGui::BeginGroup();
		if (ImGui::ImageButton(tex->Get_IMTEXTURE_ID(), { imgSize,imgSize }))
		{
			s_showView = false;
			s_assetReference->prop.set_value(s_assetReference->inst, tex);
		}
		ImGui::Text(tex->GetFileName().data());
		ImGui::EndGroup();
	}
	ImGui::EndTable();

	ImGui::End();
}


