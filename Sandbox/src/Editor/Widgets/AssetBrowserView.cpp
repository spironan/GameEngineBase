#include "AssetBrowserView.h"
#include <imgui.h>
#include "Engine/Asset/AssetsManager.h"
#include "Engine.h"
bool AssetBrowserView::s_showView = false;

AssetBrowserView::AssetReferencePoint AssetBrowserView::s_assetReference;
void AssetBrowserView::Show()
{
	if(ImGui::Begin("Asset Browser", &s_showView) == false)
	{
		ImGui::End();
		return;
	}
	//gets a list of assets
	//if the asset is selected set s_assetReference.instance to the id

	ImGui::End();
}


