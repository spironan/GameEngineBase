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
#include <rttr/type.h>
#include <rttr/property.h>

#include "RttrTypeID.h"
#include "Engine/ECS/GameObject.h"


class ProjectFolderView
{
public:
	void Show();

private:
	void ProjectView();
	void PathDir(std::filesystem::path&, std::string&);
	void SearchFilter();
	void FileBeginDrag(const std::filesystem::path&);
	void TextProcessing(std::string& string,float windowSize);
private:
	std::string m_filter;
	bool m_filtering = false;


};

