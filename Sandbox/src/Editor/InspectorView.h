#pragma once

#include <rttr/type.h>
#include <rttr/property.h>
#include <rttr/variant.h>
#include <vector>

#include "testclass.h"

class InspectorView
{
public:
	InspectorView();
	void Show();
	static void SetElementData(void*);
	static void DeleteElementData(void*);
private:

	struct ElementData
	{
	public:
		ElementData(testclass* t, const rttr::property& p, const rttr::variant& a) :item(t), prop(p), data(a) {}
		testclass* item;
		rttr::property prop;
		rttr::variant data;
	};
	void ReadComponents(const rttr::type&);
	std::vector <rttr::type::type_id> m_tracked_ids;
};

