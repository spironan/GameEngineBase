#pragma once
#include <string>
#include <vector>
class ProjectRootView
{
public:
	ProjectRootView() :m_selecteditem{ "" }, m_nameBuffer{}, delete_popup{ false }, m_rename_item{ false }
	{
		m_runOnSelected.push_back(&ProjectRootView::RenameCallback);
	};
	void Show();
private:
	void ProjectView(const std::string& path, std::string& selected_dir);
	void ProjectViewPopUp();
	void ProjectDeletePopUp_Modal();
private:
	char m_nameBuffer[128];//for renaming items
	void RenameCallback();

	std::vector<char> layer;//this is used to check if the correct item is selected as it is quicker to check a array of chars
	std::vector<char> curr;//the counter check
	std::vector<void(ProjectRootView::*)()> m_runOnSelected;//function pointers for selected item

	std::string m_selecteditem;//itemname only
	std::string m_selectedpath;//path with the itemname

	bool delete_popup;//flag for deleting modal popup
	bool m_rename_item;
};

