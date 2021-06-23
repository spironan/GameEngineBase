#include "InspectorActionBehaviour.h"

/**
 * \brief
 *		function for cleaning the data assigned
 *		some data structures might be too complex
 *		that just doing a delete on a void* will not work
 */
InspectorActionBehaviour::~InspectorActionBehaviour()
{
	undoData.clear();
	redoData.clear();
}

void InspectorActionBehaviour::undo()
{
	prop.set_value(item, undoData);
}

void InspectorActionBehaviour::redo()
{
	prop.set_value(item, redoData);
}
