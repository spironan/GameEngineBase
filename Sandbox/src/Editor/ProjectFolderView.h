/*****************************************************************//**
 * \file   ProjectFolderView.h
 * \brief  Display the items inside the selected folder from
 *		   ProjectRootView
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <filesystem>
#include <string>
#include <vector>
class testclass;
class ProjectFolderView
{
public:
	void Show();

private:
	void ProjectView();
	void SaveHeirarchy(testclass* , rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& );
	void SaveObject(testclass*, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>&);
	void PathDir(std::filesystem::path&, std::string&);
	void SearchFilter();
private:
	std::string m_filter;
	bool m_filtering = false;


};

