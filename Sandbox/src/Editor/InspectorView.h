#pragma once

#include <rttr/type.h>
#include <vector>
class InspectorView
{
public:
	InspectorView();
	void Show();
private:
	void ReadComponents(const rttr::type&);
	std::vector <rttr::type::type_id> m_tracked_ids;
};

