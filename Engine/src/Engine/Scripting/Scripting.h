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
#include "Engine/Scene/SceneManager.h"
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
        Scripting(Scripting const&);
        //Scripting(Scripting&&) = default;

        /****************************************************************************//*!
         @brief     Copies the relevant data of scripting component while retaining
                    its entityID to be of prior gameobject before this operation.
        *//*****************************************************************************/
        virtual Component& CopyComponent(Component const& comp) override;

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
        /* ECS Component Functions                                                     */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      creates and attaches a new ECS component C# interface to the GameObject

        \warning    this function should only be called during play mode as this actually
                    creates an instance of an interface in C# side, which will break if
                    a recompile is triggered

        \note       this function does not add the actual ECS component itself

        \param      name_space
                the namespace of the desired component interface (nested namespaces separated by '.')
        \param      name
                the name of the desired component interface (nested classes won't work)

        \return     the IntPtr to the newly created component interface
        *//**********************************************************************************/
        uint32_t AddComponentInterface(const char* name_space, const char* name);

        /*********************************************************************************//*!
        \brief      gets the ECS Component's C# interface from the GameObject, if any

        \warning    this function should only be called during play mode as component interface instances
                    are only created during play mode, since it will break if a recompile is triggered

        \param      name_space
                the namespace of the desired component interface (nested namespaces separated by '.')
        \param      name
                the name of the desired component interface (nested classes won't work)

        \return     the IntPtr to the desired component interface
        *//**********************************************************************************/
        uint32_t GetComponentInterface(const char* name_space, const char* name);

        template<typename Component>
        uint32_t GetComponentInterface()
        {
            ComponentType compID = SceneManager::GetActiveWorld().GetComponentType<Component>();
            if (compID >= componentList.size())
                return 0;
            return componentList[compID];
        }

        /*********************************************************************************//*!
        \brief      deletes and removes an ECS component C# interface from the GameObject, if any

        \warning    this function should only be called during play mode as component interface instances
                    are only created during play mode, since it will break if a recompile is triggered

        \note       this function does not remove the actual ECS component itself

        \param      name_space
                the namespace of the desired interface (nested namespaces separated by '.')
        \param      name
                the name of the desired interface (nested classes won't work)
        *//**********************************************************************************/
        void RemoveComponentInterface(const char* name_space, const char* name);

        /*-----------------------------------------------------------------------------*/
        /* Script Instance Functions                                                   */
        /*-----------------------------------------------------------------------------*/

        struct ScriptInstance
        {
            uint32_t handle;
            bool enabled;

            ScriptInstance(uint32_t aHandle) : handle(aHandle), enabled(true) {};
        };

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
        \brief      gets a specific script instance from the GameObject by its index

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \param      scriptID
                the index of the script instance in the GameObject's script list

        \return     a pointer to the script instance, or null if the index given is out of array size
        *//**********************************************************************************/
        ScriptInstance const* GetScript(int scriptID);

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

        /*********************************************************************************//*!
        \brief      deletes and removes a script instance by its index from the GameObject, if any

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \param      scriptID
                the index of the script instance in the GameObject's script list
        *//**********************************************************************************/
        void RemoveScript(int scriptID);

        /*********************************************************************************//*!
        \brief      enables a script instance by its C# IntPtr so that behavioural functions
                    (e.g. Update) will be executed accordingly

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \param      handle
                the IntPtr to the desired script instance to enable
        *//**********************************************************************************/
        void EnableScript(uint32_t handle);

        /*********************************************************************************//*!
        \brief      enables a script instance by its index so that behavioural functions
                    (e.g. Update) will be executed accordingly

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \param      scriptID
                the index of the script instance in the GameObject's script list
        *//**********************************************************************************/
        void EnableScript(int scriptID);

        /*********************************************************************************//*!
        \brief      disables a script instance by its C# IntPtr so that behavioural functions
                    (e.g. Update) will not be executed

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \param      handle
                the IntPtr to the desired script instance to enable
        *//**********************************************************************************/
        void DisableScript(uint32_t handle);

        /*********************************************************************************//*!
        \brief      disables a script instance by its index so that behavioural functions
                    (e.g. Update) will not be executed

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \param      scriptID
                the index of the script instance in the GameObject's script list
        *//**********************************************************************************/
        void DisableScript(int scriptID);

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
        std::map<unsigned int, ScriptInfo>& GetScriptInfoAll();

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

        /*********************************************************************************//*!
        \brief      refreshes all ScriptInfo in the Scripting component in the event
                    of any changes made to the actual after compilation
        *//**********************************************************************************/
        void RefreshScriptInfoAll();

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
        \brief      invokes a script instance's function by name with the given parameters

        \warning    this should only be called during play mode as no script instances
                    exist during edit mode to invoke functions from.

        \param      pointer
                the C# IntPtr to the target script instance
        \param      functionName
                the name of the function to invoke
        \param      paramCount
                the number of parameters the function takes in
        \param      params
                the pointer to the void* array containing all the parameters
        *//**********************************************************************************/
        static void InvokeFunction(uint32_t pointer, const char* functionName, int paramCount = 0, void** params = NULL);

        /*********************************************************************************//*!
        \brief      invokes a function by name in all script instances attached to this GameObject
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
            void* params[] = { (void*)(&args)... };
            InvokeFunctionAll(functionName, paramCount, params);
        }

        /*********************************************************************************//*!
        \brief      invokes the OnTriggerEnter2D function in all script instances attached to this GameObject

        \warning    this should only be called during play mode as no script instances
                    exist during edit mode to invoke functions from.

        \param      other
                the scripting component of the other gameObject that was triggered
        *//**********************************************************************************/
        void InvokeTriggerEnter2D(Scripting& other);

        /*********************************************************************************//*!
        \brief      invokes the OnTriggerStay2D function in all script instances attached to this GameObject

        \warning    this should only be called during play mode as no script instances
                    exist during edit mode to invoke functions from.

        \param      other
                the scripting component of the other gameObject that was triggered
        *//**********************************************************************************/
        void InvokeTriggerStay2D(Scripting& other);

        /*********************************************************************************//*!
        \brief      invokes the OnTriggerExit2D function in all script instances attached to this GameObject

        \warning    this should only be called during play mode as no script instances
                    exist during edit mode to invoke functions from.

        \param      other
                the scripting component of the other gameObject that was triggered
        *//**********************************************************************************/
        void InvokeTriggerExit2D(Scripting& other);

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
        std::vector<uint32_t> componentList;
        std::vector<ScriptInstance> scriptList;
        std::map<unsigned int, ScriptInfo> scriptInfoMap;
    };
}