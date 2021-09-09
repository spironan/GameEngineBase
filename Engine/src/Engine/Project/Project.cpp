/************************************************************************************//*!
\file           Project.cpp
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

    void Project::SaveCurrentProject()
    {
        ENGINE_ASSERT(s_activeProject);
        ProjectSerializer serializer(s_activeProject);
        serializer.Deserialize(s_activeProject.get()->m_config.ProjectDirectory);
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
