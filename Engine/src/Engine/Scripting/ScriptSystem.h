/************************************************************************************//*!
\file           ScriptSystem.h
\project        <PROJECT_NAME>
\author         Solomon Tan Teng Shue, t.tengshuesolomon, 620010020
\par            email: t.tengshuesolomon\@digipen.edu
\date           August 4, 2021
\brief          Defines the Script System that is responsible for C# compiling during runtime
                and for executing instructions for all scripts in all existing GameObjects

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/ECS/System.h"
#include "ScriptInfo.h"
#include "Scripting.h"

#include <functional>

namespace engine
{
    class ScriptSystem final : public System
    {

    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors & Destructor                                                   */
        /*-----------------------------------------------------------------------------*/
        explicit ScriptSystem(ECS_Manager& _ECS_Manager) : System{ _ECS_Manager } {};

        /*-----------------------------------------------------------------------------*/
        /* Compiling                                                                   */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      loads the currently compiled scripts, if any. This should be called only
                    after registering all components into the ScriptSystem
        *//**********************************************************************************/
        static void Initialize();

        /*********************************************************************************//*!
        \brief      executes the required actions to compile all user made scripts during runtime
        *//**********************************************************************************/
        static void Compile();

        /*********************************************************************************//*!
        \brief      executes the required mono functions to clean up resources used by mono
        *//**********************************************************************************/
        static void CleanUp();

        /*********************************************************************************//*!
        \brief      Checks if the C# scripts have been properly compiled and the script system
                    is properly set up and ready for use

        \return     true if it has been set up and is ready for use, else false
        *//**********************************************************************************/
        static bool IsSetUp();

        /*********************************************************************************//*!
        \brief      Gets the list of all C# scripts compiled

        \note       C# scripts are identified by inheriting the MonoBehaviour C# class

        \return     the list of all c# scripts compiled
        *//**********************************************************************************/
        static std::vector<ScriptClassInfo> const& GetScriptClassList();

        /*********************************************************************************//*!
        \brief      refreshes all ScriptInfo in all GameObjects in the event
                    of any changes made to the actual after compilation
        *//**********************************************************************************/
        void RefreshScriptInfoAll();

        /*-----------------------------------------------------------------------------*/
        /* ECS Components                                                              */
        /*-----------------------------------------------------------------------------*/
        
        struct RegisteredComponent
        {
        public:
            std::function<void(Entity)> Add;
            std::function<bool(Entity)> Has;
            std::function<ComponentType(void)> Index;
            std::function<void(Entity)> Remove;
        };

        /*********************************************************************************//*!
        \brief      Registers an ECS Component with its corresponding C# interface class name
                    to the ScriptSystem

        \param      interfaceTypeName
                the name of the C# interface type, with namespaces separated by '.'
                (e.g. Ouroboros.Transform)
        *//**********************************************************************************/
        template<typename Component>
        static void RegisterComponent(std::string const& interfaceTypeName)
        {
            std::function<void(Entity)> add = [](Entity entity)
            {
                GameObject obj{ entity };
                obj.AddComponent<Component>();
            };
            std::function<bool(Entity)> has = [](Entity entity)
            {
                GameObject obj{ entity };
                return obj.HasComponent<Component>();
            };
            std::function<ComponentType(void)> index = [](void)
            {
                return SceneManager::GetActiveWorld().GetComponentType<Component>();
            };
            std::function<void(Entity)> remove = [](Entity entity)
            {
                GameObject obj{ entity };
                return obj.RemoveComponent<Component>();
            };
            size_t id = s_ComponentMap.size();
            s_ComponentMap.insert({ interfaceTypeName, { add, has, index, remove } });
        }

        /*-----------------------------------------------------------------------------*/
        /* Mode Functions                                                              */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      executes the necessary functions in all Scripting components to initiate play mode

        \return     true if play mode was successfully initiated, else false
        *//**********************************************************************************/
        bool StartPlay();

        /*********************************************************************************//*!
        \brief      executes the necessary functions in all Scripting components to stop play mode

        \return     true if play mode was successfully stopped, else false
        *//**********************************************************************************/
        bool StopPlay();

        /*-----------------------------------------------------------------------------*/
        /* Function Invoking                                                           */
        /*-----------------------------------------------------------------------------*/
        /*********************************************************************************//*!
        \brief      invokes a function by name in all script instances of all GameObjects
                    with the given parameters

        \warning    this should only be called during play mode as no script instances
                    exist during edit mode to invoke functions from.

        \param      functionName
                the name of the function to invoke
        \param      paramCount
                the number of parameters the function takes in
        \param      params
                the pointer to the void* array containing all the parameters
        *//**********************************************************************************/
        void InvokeFunctionAll(const char* functionName, int paramCount = 0, void** params = NULL);

        /*********************************************************************************//*!
        \brief      invokes a function by name in all script instances attached to this GameObject
                    with the given parameters

        \warning    this should only be called during play mode as no script instances
                    exist during edit mode to invoke functions from.

        \param      functionName
                the name of the function to invoke
        \param      paramCount
                the number of parameters the function takes in
        \param      args
                the parameters to pass to the script function
        *//**********************************************************************************/
        template<typename ... Args>
        void InvokeFunctionAll(const char* functionName, int paramCount, Args... args)
        {
            if (!s_IsPlaying)
                return;
            for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
            {
                scripting.InvokeFunctionAll(functionName, paramCount, args...);
            }
        }

        /*-----------------------------------------------------------------------------*/
        /* Debugging Tools                                                             */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      outputs all GameObjects' script info (edit mode info) 
                    to the standard output stream for debugging purposes
        *//**********************************************************************************/
        void DebugPrintInfo();

        /*********************************************************************************//*!
        \brief      outputs all GameObjects' script instance info (play mode info)
                    to the standard output stream for debugging purposes
        *//**********************************************************************************/
        void DebugPrint();

    private:
        static bool s_IsPlaying;
        static std::unordered_map<std::string, RegisteredComponent> s_ComponentMap;
    };
}