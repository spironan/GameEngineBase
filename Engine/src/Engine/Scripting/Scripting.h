/************************************************************************************//*!
\file           Scripting.h
\project        <PROJECT_NAME>
\author         Solomon Tan Teng Shue, t.tengshuesolomon, 620010020
\par            email: t.tengshuesolomon\@digipen.edu
\date           August 4, 2021
\brief          Defines a Scripting Component, which allows GameObjects to be attached
                with C# scripts written by users

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <vector>

#include "Engine/ECS/Component.h"
#include "ScriptInfo.h"

namespace engine
{
    class Scripting final : public Component
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        Scripting() = delete;
        Scripting(Scripting const&) = default;
        Scripting(Scripting&&) = default;

        /*********************************************************************************//*!
        \brief      overloaded constructor for Scripting component
         
        \param      entityID
                the id of the entity this belongs to
        \param      active
                boolean that determines if the component is active or not
        *//**********************************************************************************/
        Scripting(Entity entityID, bool active = true);

        virtual ~Scripting() override;

        Scripting& operator=(Scripting const&) = default;
        Scripting& operator=(Scripting&&) = default;

        /*-----------------------------------------------------------------------------*/
        /* Script Functions                                                            */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      creates and attaches a new script instance of a given C# class to the GameObject

        \warning    this function should only be called during play mode as this actually
                    creates an instance of a script in C# side, which will break if
                    a recompile is triggered
         
        \param      name_space
                the namespace of the desired script (nested namespaces separated by '.')
        \param      name
                the name of the desired script (nested classes won't work)
        \param      callAwake
                determines if the script's Awake function is to be called
        
        \return     the IntPtr to the newly created script
        *//**********************************************************************************/
        uint32_t AddScript(const char* name_space, const char* name, bool callAwake = true);

        /*********************************************************************************//*!
        \brief      gets a script instance of a given C# class from the GameObject, if any

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \param      name_space
                the namespace of the desired script (nested namespaces separated by '.')
        \param      name
                the name of the desired script (nested classes won't work)

        \return     the IntPtr to the desired script instance, 0 if not found
        *//**********************************************************************************/
        uint32_t GetScript(const char* name_space, const char* name);

        /*********************************************************************************//*!
        \brief      deletes and removes a script instance of a given C# class from the GameObject, if any

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered
         
        \param      name_space
                the namespace of the desired script (nested namespaces separated by '.')
        \param      name
                the name of the desired script (nested classes won't work)
        *//**********************************************************************************/
        void RemoveScript(const char* name_space, const char* name);

        /*-----------------------------------------------------------------------------*/
        /* Script Info Functions                                                       */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      adds a new script info set that will be used to create a script instance during play mode

        \warning    while this won't break anything, since script info is only used at the
                    beginning of play mode, this function should only be called during
                    edit mode as any changes in play mode won't have any immediate effects
                    on the current play session
         
        \param      classInfo
                the class info of the desired script
        
        \return     a reference to the newly created script info set
        *//**********************************************************************************/
        ScriptInfo& AddScriptInfo(ScriptClassInfo const& classInfo);

        /*********************************************************************************//*!
        \brief      gets a script info set based on the given class info

        \warning    while this won't break anything, since script info is only used at the
                    beginning of play mode, this function should only be called during
                    edit mode as any changes in play mode won't have any immediate effects
                    on the current play session
         
        \param      classInfo
                the class info of the desired script info
        
        \return     a pointer to the desired script info set, or null if no matching info was found
        *//**********************************************************************************/
        ScriptInfo* GetScriptInfo(ScriptClassInfo const& classInfo);

        /*********************************************************************************//*!
        \brief      gets a list of all script info of the GameObject

        \warning    while this won't break anything, since script info is only used at the
                    beginning of play mode, this function should only be called during
                    edit mode as any changes in play mode won't have any immediate effects
                    on the current play session
        
        \return     a reference to the vector of all script info of the current GameObject
        *//**********************************************************************************/
        std::vector<ScriptInfo>& GetScriptInfoAll();

        /*********************************************************************************//*!
        \brief      removes a script info set based on the given class info
         
        \warning    while this won't break anything, since script info is only used at the
                    beginning of play mode, this function should only be called during
                    edit mode as any changes in play mode won't have any immediate effects
                    on the current play session

        \param      classInfo
                the class info of the script info to be removed
        *//**********************************************************************************/
        void RemoveScriptInfo(ScriptClassInfo const& classInfo);

        /*-----------------------------------------------------------------------------*/
        /* Mode Functions                                                              */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      creates the GameObject instance and all required script instances in C# side
                    from the GameObject's script info list without calling their awake function yet
                    in preparation for starting play mode

        \warning    the function StartPlay should be called after this to fully start play mode,
                    unless the script component was created during play mode, in which case SetUpPlay is enough
        *//**********************************************************************************/
        void SetUpPlay();

        /*********************************************************************************//*!
        \brief      assign script info variables set during edit mode to the script instances
                    in C# side and call all script instances' awake function to fully start play mode

        \warning    this function should only be called after calling SetUpPlay in prepartion
                    for starting play mode, unless the script component was created during play mode,
                    in which case SetUpPlay is enough
        *//**********************************************************************************/
        void StartPlay();

        /*********************************************************************************//*!
        \brief      deletes the GameObject instance and all script instances of the GameObject
                    in C# side as clean up for exiting play mode
        *//**********************************************************************************/
        void StopPlay();

        /*-----------------------------------------------------------------------------*/
        /* Function Invoking                                                           */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      invokes a function by name in all script instances attached to this GameObject

        \warning    this should only be called during play mode as no script instances
                    exist during edit mode to invoke functions from.
         
        \param      functionName
                the name of the function to invoke
        *//**********************************************************************************/
        void InvokeFunctionAll(const char* functionName);

        /*-----------------------------------------------------------------------------*/
        /* Getters                                                                     */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      gets the IntPtr to the GameObject instance of this GameObject in C# side
        
        \return     the IntPtr to the C# GameObject instance
        *//**********************************************************************************/
        uint32_t GetGameObjectPtr();

        /*-----------------------------------------------------------------------------*/
        /* Debugging Tools                                                             */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      outputs all of the GameObject's script info (edit mode info) 
                    to the standard output stream for debugging purposes
        *//**********************************************************************************/
        void DebugPrintInfo();

        /*********************************************************************************//*!
        \brief      outputs all of the GameObject's script instance info (play mode info)
                    to the standard output stream for debugging purposes
        *//**********************************************************************************/
        void DebugPrint();

    private:
        uint32_t gameObjPtr;
        std::vector<uint32_t> scriptList;
        std::vector<ScriptInfo> scriptInfoList;
    };

    /*-----------------------------------------------------------------------------*/
    /* Script Functions for C#                                                     */
    /*-----------------------------------------------------------------------------*/
    extern "C"
    {
        /*********************************************************************************//*!
        \brief      creates and attaches a new script instance of a given C# class to a specific GameObject.
                    This function's main purpose is to expose the Scripting component's 
                    AddScript function to the C# side

        \warning    this function should only be called during play mode as this actually
                    creates an instance of a script in C# side, which will break if
                    a recompile is triggered

        \param      id
                the entity id of the target GameObject
        \param      name_space
                the namespace of the desired script (nested namespaces separated by '.')
        \param      name
                the name of the desired script (nested classes won't work)

        \return     the IntPtr to the newly created script
        *//**********************************************************************************/
        __declspec(dllexport) uint32_t AddScript(int id, const char* name_space, const char* name);

        /*********************************************************************************//*!
        \brief      gets a script instance of a given C# class from the GameObject, if any.
                    This function's main purpose is to expose the Scripting component's
                    GetScript function to the C# side

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered
        
        \param      id
                the entity id of the target GameObject
        \param      name_space
                the namespace of the desired script (nested namespaces separated by '.')
        \param      name
                the name of the desired script (nested classes won't work)

        \return     the IntPtr to the desired script instance, 0 if not found
        *//**********************************************************************************/
        __declspec(dllexport) uint32_t GetScript(int id, const char* name_space, const char* name);

        /*********************************************************************************//*!
        \brief      deletes and removes a script instance of a given C# class from the GameObject, if any.
                    This function's main purpose is to expose the Scripting component's
                    RemoveScript function to the C# side

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \param      id
                the entity id of the target GameObject
        \param      name_space
                the namespace of the desired script (nested namespaces separated by '.')
        \param      name
                the name of the desired script (nested classes won't work)
        *//**********************************************************************************/
        __declspec(dllexport) void RemoveScript(int id, const char* name_space, const char* name);
    }
}