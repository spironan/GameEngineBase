#pragma once

#include "Engine/Core/Base.h"
#include <filesystem>

namespace engine
{
    struct ProjectConfig
    {
        std::string Name;

        std::string StartScene;
        
        std::string AssetDirectory;
        std::string ScriptModulePath;

        // Not serialized
        std::string ProjectDirectory;
    };

    class Project
    {
    public:
        Project() = default;
        ~Project() = default;

        const ProjectConfig& GetConfig() const { return m_config; }

        static void CreateProject(std::string const& projectPath);
        static void LoadProject(std::string const& projectPath);

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
