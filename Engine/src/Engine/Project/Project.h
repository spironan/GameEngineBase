/************************************************************************************//*!
\file           Project.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Sept 09, 2021
\brief          Project describes a location in space that can used to store relevant
                Information about the project that can be loaded and saved.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Base.h"
#include <filesystem>

namespace engine
{
    /********************************************************************************//*!
     @brief     Describes useful information can be used and 
                that will be stored in config file
    *//*********************************************************************************/
    struct ProjectConfig
    {
        std::string Name;

        std::string StartScene;
        
        std::string AssetDirectory;

        std::string ScriptModulePath;

        // Not serialized
        std::string ProjectDirectory;
    };

    /********************************************************************************//*!
     @brief     Describes what a project is and functionality to create, load and save
                Project files
    *//*********************************************************************************/
    class Project
    {
    public:
        Project() = default;
        ~Project() = default;

        const ProjectConfig& GetConfig() const { return m_config; }

        static void CreateProject(std::string const& projectPath);
        static void LoadProject(std::string const& projectPath);
        static void SaveCurrentProject();

        static std::shared_ptr<Project> GetActiveProject() { return s_activeProject; }
        static void SetActiveProject(std::shared_ptr<Project> project) { s_activeProject = project; };

        static const std::string& GetProjectName()
        {
            ENGINE_ASSERT(s_activeProject);
            return s_activeProject->GetConfig().Name;
        }

        static std::filesystem::path GetProjectDirectory()
        {
            ENGINE_ASSERT(s_activeProject);
            return s_activeProject->GetConfig().ProjectDirectory;
        }

        static std::filesystem::path GetAssetDirectory()
        {
            ENGINE_ASSERT(s_activeProject);
            return std::filesystem::path(s_activeProject->GetConfig().ProjectDirectory) / s_activeProject->GetConfig().AssetDirectory;
        }

        static std::filesystem::path GetScriptModulePath()
        {
            ENGINE_ASSERT(s_activeProject);
            return std::filesystem::path(s_activeProject->GetConfig().ProjectDirectory) / s_activeProject->GetConfig().ScriptModulePath;
        }

    private:
        ProjectConfig m_config;

        static std::shared_ptr<Project> s_activeProject;
    };

    class ProjectSerializer
    {
    public:
        ProjectSerializer(std::shared_ptr<Project> project);

        bool Serialize(std::string const& fileapth);
        bool Deserialize(std::string const& filepath);

    private:
        std::shared_ptr<Project> m_project;
    };
}
