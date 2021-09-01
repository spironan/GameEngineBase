#pragma once

#include "Engine/ECS/ECS_Utility.h"
#include "Editor/ActionStack/ActionBehaviour.h"
#include "Engine/ECS/DeletedGameObject.h"


class DeleteItemActionStack: public ActionBehaviour
{
public:
	DeleteItemActionStack(const std::string& desc, engine::Entity object);
	~DeleteItemActionStack();
	virtual void undo() override;
	virtual void redo() override;

private:
	engine::Entity m_orignalParent = 0;
	engine::Entity m_undoData = 0;
	std::vector<std::shared_ptr<engine::DeletedGameObject>> m_redoData;
	std::vector<unsigned> m_sparseHierarchy;
};
