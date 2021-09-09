#include "pch.h"
#include "Project.h"

namespace engine
{
    void Project::CreateProject(std::string const& projectPath)
    {
        std::shared_ptr<Project> project = std::make_shared<Project>();
        ProjectSerializer serializer(project);
        serializer.Serialize(projectPath);
        SetActiveProject(project);
    }

    void Project::LoadProject(std::string const& projectPath)
    {
        std::shared_ptr<Project> project = std::make_shared<Project>();
        ProjectSerializer serializer(project);
        serializer.Deserialize(projectPath);
        SetActiveProject(project);
    }

    ProjectSerializer::ProjectSerializer(std::shared_ptr<Project> project)
        : m_project{ project }
    {
    }

    bool ProjectSerializer::Serialize(std::string const& fileapth)
    {
        // function not implemented yet.
        ENGINE_ASSERT(false);
        return false;
    }

    bool ProjectSerializer::Deserialize(std::string const& filepath)
    {
        // function not implemented yet.
        ENGINE_ASSERT(false);
        return false;
    }
}
