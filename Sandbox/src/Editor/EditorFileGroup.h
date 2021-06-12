#pragma once
#include <string>
#include <imgui.h>
class FileGroup
{
public:
	FileGroup() {};
	~FileGroup() {};

	static std::string s_rootPath;
	static std::string s_CurrentPath;
	static std::string s_hoveredPath;
	static std::string s_selectedpath;//path with the itemname
	static std::string s_selecteditem;//itemname only
	static ImVec2 s_selectedItemPosition;//vec2
	static char s_nameBuffer[128];//for renaming items

	static ImGuiID s_projectviewid;

	static bool s_delete_popup;//flag for deleting modal popup

	static bool s_rename_item;
public:

	static void ProjectViewPopUp();

private:
	static void ProjectViewPopUpOptions();
	static void RenamePopUp();
	static void DeletePopUp();
private:

};

