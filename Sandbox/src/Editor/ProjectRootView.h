#pragma once
#include <string>
#include <vector>
class ProjectRootView
{
public:
	ProjectRootView() :m_selecteditem{ "" }, delete_popup{ false } {};
	void Show();
private:
	void ProjectView(const std::string& path, std::string& selected_dir);
	void ProjectViewPopUp();
	void ProjectDeletePopUp_Modal();
private:
	std::vector<char> layer;//this is used to check if the correct item is selected as it is quicker to check a array of chars
	std::vector<char> curr;//the counter check
	std::string m_selecteditem;//itemname only
	std::string m_selectedpath;//path with the itemname
	bool delete_popup;
};

