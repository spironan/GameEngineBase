#pragma once
#include "rttr/instance.h"
#include "rttr/property.h"
#include "Engine.h"
class AssetBrowserView
{
public:
	AssetBrowserView() = default;
	~AssetBrowserView() = default;

	void Show();
	template<typename Component>
	static void OpenAssetBroswer(rttr::property& prop, Component& component);
private:
	struct AssetReferencePoint
	{
		AssetReferencePoint() = default;
		AssetReferencePoint(const AssetReferencePoint &) = default;
		AssetReferencePoint& operator= (AssetReferencePoint const&) = default;
		AssetReferencePoint& operator= (AssetReferencePoint &&) = default;
		AssetReferencePoint(rttr::property& property, rttr::instance& instance) :prop{ property },inst{ instance }{};
		rttr::property prop;
		rttr::instance inst;
	};
	static AssetReferencePoint s_assetReference;
	static bool s_showView;
};

template<typename Component>
inline void AssetBrowserView::OpenAssetBroswer(rttr::property& prop, Component& component)
{
	s_showView = true;
	s_assetReference.inst(component);
	s_assetReference.prop = prop;
}
