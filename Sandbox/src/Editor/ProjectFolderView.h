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

private:
	

};

