#pragma once
#include "pch.h"
#include "testclass.h"
#include <rapidjson/ostreamwrapper.h>//for ostreamwrapper
#include <rapidjson/prettywriter.h>//for prettywriter
enum class GUIACTIVE_FLAGS
{
	INSPECTOR_ACTIVE = 1,
	PROPERTYEDITOR_ACTIVE = 2,
	PROJECTHIRECHY_ACTIVE = 4,
	PROJECTVIEW_ACTIVE = 8
};
class Editor
{
public:
	Editor();
	~Editor();
	//uses the (or) operator to set the flag
	void SetGUIActive(GUIACTIVE_FLAGS flag) { activeFlagGUI |= static_cast<int>(flag); }
	//uses the (xor) operator to set the flag
	void SetGUIInactive(GUIACTIVE_FLAGS flag) { activeFlagGUI ^= static_cast<int>(flag); }

	void UpdateUI();
	void TestFunction();
private:
	testclass* ShowObject(testclass& tc);
	void PathDir(std::filesystem::path& entry, std::string& path);
	void ProjectFile(const std::string& path, std::string& selected_dir);
	void PreviewFolder(std::string& path);

	void SaveHirechy(testclass& tc, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer);
	void SaveObject(testclass& tc, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer);
	void SaveData();
	
	void LoadData(const char* dir);
	void ReadData(testclass* data);

private:
	std::vector<testclass> testList;
	int activeFlagGUI = 0;
	std::string currentPath;
};