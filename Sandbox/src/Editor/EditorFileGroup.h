#pragma once
#include <string>
class FileGroup
{
public:
	FileGroup() {};
	~FileGroup() {};

	static std::string s_CurrentPath;
	static std::string s_rootPath;
	static std::string s_hoveredPath;

	static void ProjectViewPopUp();
private:

};

