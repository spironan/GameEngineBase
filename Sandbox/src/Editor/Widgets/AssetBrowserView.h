#pragma once
#include "rttr/property.h"
#include "rttr/type.h"
#include "rttr/variant.h"
#include "rttr/variant_array_view.h"
#include "rttr/instance.h"
#include "Engine.h"
class AssetBrowserView
{
public:
	AssetBrowserView() = default;
	~AssetBrowserView() { delete s_assetReference; };

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
	static AssetReferencePoint* s_assetReference;
	static bool s_showView;
};

template<typename Component>
inline void AssetBrowserView::OpenAssetBroswer(rttr::property& prop, Component& component)
{
	s_showView = true;
	delete s_assetReference;
	s_assetReference = new AssetReferencePoint(prop, rttr::instance(component));
}
