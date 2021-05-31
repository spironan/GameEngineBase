#pragma once
#include <string>
#include <vector>
class ProjectRootView
{
public:
	ProjectRootView() :m_selecteditem{""} {};
	void Show();
private:
	void ProjectView(const std::string& path, std::string& selected_dir);
	void ProjectViewPopUp();
private:
	std::vector<char> layer;
	std::vector<char> curr;
	std::string m_selecteditem;
};

