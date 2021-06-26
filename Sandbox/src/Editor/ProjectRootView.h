/*****************************************************************//**
 * \file   ProjectRootView.h
 * \brief  Display the path of all the folders
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
 *********************************************************************/
#pragma once
#include <string>
#include <vector>
class ProjectRootView
{
public:
	ProjectRootView() {};
	void Show();
private:
	void ProjectView(const std::string& path, std::string& selected_dir);
private:
	

	std::vector<char> layer;//this is used to check if the correct item is selected as it is quicker to check a array of chars
	std::vector<char> curr;//the counter check




};

