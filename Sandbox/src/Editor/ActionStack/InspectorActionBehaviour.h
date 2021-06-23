#pragma once
#include "ActionBehaviour.h"
#include "testclass.h"

#include <rttr/property.h>
#include <rttr/variant.h>
class InspectorActionBehaviour : public ActionBehaviour
{
public:
	InspectorActionBehaviour(testclass* tc,const rttr::property& p, rttr::variant& _undo,rttr::variant& _redo) 
		:ActionBehaviour{}, item(tc), prop(p), undoData(_undo),redoData(_redo) {}
	virtual ~InspectorActionBehaviour()override;
	void undo()override;
	void redo()override;

private:
	testclass* item;
	rttr::property prop;
	rttr::variant undoData;
	rttr::variant redoData;
};

