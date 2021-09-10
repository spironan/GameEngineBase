#pragma once

#include "ScriptUtilityAPI.h"
#include "ScriptUtility.h"

namespace engine
{
    namespace scriptAPI
    {
        /*-----------------------------------------------------------------------------*/
        /* Entity Functions for C#                                                     */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      Creates a new GameObject in the currently active world.
                    This function's main purpose is to enable scripts in the C# side to create entities
                    during play mode

        \warning    this function should only be called during play mode as instances in C# side
                    are created in this function, which will break if a recompile is triggered

        \return     the IntPtr to the newly created GameObject in C# side
        *//**********************************************************************************/
        SCRIPT_API Entity CreateEntity();

        /*********************************************************************************//*!
        \brief      Creates a copy of a given source GameObject in the currently active world.
                    This function's main purpose is to enable scripts in the C# side to instantiate entities
                    during play mode

        \warning    this function should only be called during play mode as instances in C# side
                    are created in this function, which will break if a recompile is triggered

        \return     the IntPtr to the newly created GameObject in C# side
        *//**********************************************************************************/
        SCRIPT_API uint32_t InstantiateEntity(Entity src);

        /*********************************************************************************//*!
        \brief      Removes a GameObject in the currently active world with a specific entity id.
                    This function's main purpose is to enable scripts in the C# side to remove entities
                    during play mode

        \param      id
                the entity id of the GameObject to be destroyed
        *//**********************************************************************************/
        SCRIPT_API void DestroyEntity(Entity id);

        /*-----------------------------------------------------------------------------*/
        /* Script Functions for C#                                                     */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      gets the name of a GameObject specified by its entity id

        \note       Mainly used for C# side calling

        \param      id
                the entity id of the target GameObject

        \return     the IntPtr to the newly created MonoString (C# string) containing the name
        *//**********************************************************************************/
        SCRIPT_API uint32_t GameObject_GetName(Entity id);

        /*********************************************************************************//*!
        \brief      sets the name of a GameObject specified by its entity id

        \note       Mainly used for C# side calling

        \param      id
                the entity id of the target GameObject

        \param      newName
                the GameObject's new name
        *//**********************************************************************************/
        SCRIPT_API void GameObject_SetName(Entity id, const char* newName);

        /*********************************************************************************//*!
        \brief      gets the active state of a GameObject specified by its entity id

        \note       Mainly used for C# side calling

        \param      id
                the entity id of the target GameObject

        \return     true if it is active, else false
        *//**********************************************************************************/
        SCRIPT_API bool GameObject_GetActive(Entity id);

        /*********************************************************************************//*!
        \brief      sets the active state of a GameObject specified by its entity id

        \note       Mainly used for C# side calling

        \param      id
                the entity id of the target GameObject

        \param     activeSelf
                the desired active state of the GameObject
        *//**********************************************************************************/
        SCRIPT_API void GameObject_SetActive(Entity id, bool value);

        /*********************************************************************************//*!
        \brief      creates and attaches a new script instance of a given C# class to a specific GameObject.

        \warning    this function should only be called during play mode as this actually
                    creates an instance of a script in C# side, which will break if
                    a recompile is triggered

        \note       Mainly used for C# side calling

        \param      id
                the entity id of the target GameObject
        \param      name_space
                the namespace of the desired script (nested namespaces separated by '.')
        \param      name
                the name of the desired script (nested classes won't work)

        \return     the IntPtr to the newly created script
        *//**********************************************************************************/
        SCRIPT_API uint32_t AddScript(Entity id, const char* name_space, const char* name);

        /*********************************************************************************//*!
        \brief      gets a script instance of a given C# class from the GameObject, if any.

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \note       Mainly used for C# side calling

        \param      id
                the entity id of the target GameObject
        \param      name_space
                the namespace of the desired script (nested namespaces separated by '.')
        \param      name
                the name of the desired script (nested classes won't work)

        \return     the IntPtr to the desired script instance, 0 if not found
        *//**********************************************************************************/
        SCRIPT_API uint32_t GetScript(Entity id, const char* name_space, const char* name);

        /*********************************************************************************//*!
        \brief      deletes and removes a script instance of a given C# class from the GameObject, if any.

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \note       Mainly used for C# side calling

        \param      id
                the entity id of the target GameObject
        \param      name_space
                the namespace of the desired script (nested namespaces separated by '.')
        \param      name
                the name of the desired script (nested classes won't work)
        *//**********************************************************************************/
        SCRIPT_API void RemoveScript(Entity id, const char* name_space, const char* name);

        /*********************************************************************************//*!
        \brief      deletes and removes a script instance by its index from the GameObject, if any.

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \note       Mainly used for C# side calling

        \param      entityID
                the entity id of the target GameObject
        \param      scriptID
                the index of the desired script instance in the target GameObject's script list
        *//**********************************************************************************/
        SCRIPT_API void DestroyScript(Entity entityID, int scriptID);

        /*********************************************************************************//*!
        \brief      sets the active state of a given script instance so that behavioural functions
                    (e.g. Update) will be executed accordingly

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \note       Mainly used for C# side calling

        \param      entityID
                the entity id of the target GameObject
        \param      scriptID
                the index of the desired script instance in the target GameObject's script list
        \param      enabled
                the desired active state of the script instance
        *//**********************************************************************************/
        SCRIPT_API void SetScriptEnabled(Entity entityID, int scriptID, bool enabled);

        /*********************************************************************************//*!
        \brief      gets the active state of a given script instance

        \warning    this function should only be called during play mode as script instances
                    are only created during play mode, since it will break if a recompile is triggered

        \note       Mainly used for C# side calling

        \param      entityID
                the entity id of the target GameObject
        \param      scriptID
                the index of the desired script instance in the target GameObject's script list

        \return     the active state of the specified script instance
        *//**********************************************************************************/
        SCRIPT_API bool CheckScriptEnabled(Entity entityID, int scriptID);

        /*********************************************************************************//*!
        \brief      creates and attaches a new ECS component and its corresponding C# interface
                    to a specific GameObject.

        \warning    this function should only be called during play mode as this actually
                    creates an instance of an interface in C# side, which will break if
                    a recompile is triggered

        \note       Mainly used for C# side calling

        \param      id
                the entity id of the target GameObject
        \param      name_space
                the namespace of the desired ECS Component's C# interface (nested namespaces separated by '.')
        \param      name
                the name of the desired ECS Component's C# interface (nested classes won't work)

        \return     the IntPtr to the newly created component interface
        *//**********************************************************************************/
        SCRIPT_API uint32_t AddComponentFromScript(Entity id, const char* name_space, const char* name);

        /*********************************************************************************//*!
        \brief      gets the ECS Component's C# interface from the GameObject, if any

        \warning    this function should only be called during play mode as component interface instances
                    are only created during play mode, since it will break if a recompile is triggered

        \note       Mainly used for C# side calling

        \param      id
                the entity id of the target GameObject
        \param      name_space
                the namespace of the desired component interface (nested namespaces separated by '.')
        \param      name
                the name of the desired component interface (nested classes won't work)

        \return     the IntPtr to the desired component interface
        *//**********************************************************************************/
        SCRIPT_API uint32_t GetComponentFromScript(Entity id, const char* name_space, const char* name);

        /*********************************************************************************//*!
        \brief      deletes and removes an ECS component and its corresponding C# interface
                    from the GameObject, if any

        \warning    this function should only be called during play mode as component interface instances
                    are only created during play mode, since it will break if a recompile is triggered

        \note       Mainly used for C# side calling

        \param      id
                the entity id of the target GameObject
        \param      name_space
                the namespace of the desired ECS Component's C# interface (nested namespaces separated by '.')
        \param      name
                the name of the desired ECS Component's C# interface (nested classes won't work)
        *//**********************************************************************************/
        SCRIPT_API void RemoveComponentFromScript(Entity id, const char* name_space, const char* name);

        /*-----------------------------------------------------------------------------*/
        /* Input Functions for C#                                                      */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      Determine if any supported key has been triggered during this frame

        \note       Mainly used for C# side calling

        \return     true if any supported key was triggered during this frame, else false
        *//**********************************************************************************/
        SCRIPT_API bool IsAnyKeyPressed();

        /*********************************************************************************//*!
        \brief      Determine if any supported key is currently held down.

        \note       Mainly used for C# side calling

        \return     true if any supported key is currently held down, else false
        *//**********************************************************************************/
        SCRIPT_API bool IsAnyKeyHeld();

        /*********************************************************************************//*!
        \brief      Determine if any supported key has been released during this frame

        \note       Mainly used for C# side calling

        \return     true if any supported key was released during this frame, else false
        *//**********************************************************************************/
        SCRIPT_API bool IsAnyKeyReleased();

        /*********************************************************************************//*!
        \brief      Determine if a supported key by the given keycode has been triggered during this frame

        \note       Mainly used for C# side calling

        \return     true if the specified key was triggered during this frame, else false
        *//**********************************************************************************/
        SCRIPT_API bool IsKeyPressed(int key);

        /*********************************************************************************//*!
        \brief      Determine if a supported key by the given keycode is currently held down

        \note       Mainly used for C# side calling

        \return     true if the specified key is currently held down, else false
        *//**********************************************************************************/
        SCRIPT_API bool IsKeyHeld(int key);

        /*********************************************************************************//*!
        \brief      Determine if a supported key by the given keycode has been released during this frame

        \note       Mainly used for C# side calling

        \return     true if the specified key was released during this frame, else false
        *//**********************************************************************************/
        SCRIPT_API bool IsKeyReleased(int key);

        /*********************************************************************************//*!
        \brief      Retrieves the current mouse position in screen coordinates
                    (top left 0,0)

        \note       Mainly used for C# side calling

        \param      x
                the pointer to set the x-coordinate of the mouse screen position to

        \param      y
                the pointer to set the y-coordinate of the mouse screen position to
        *//**********************************************************************************/
        SCRIPT_API void GetMousePosition(int* x, int* y);

        /*********************************************************************************//*!
        \brief      Retrieves the change in mouse position in the current frame in screen coordinates
                    (top left 0,0)

        \note       Mainly used for C# side calling

        \param      x
                the pointer to set the x-coordinate of the mouse screen position change to

        \param      y
                the pointer to set the y-coordinate of the mouse screen position change to
        *//**********************************************************************************/
        SCRIPT_API void GetMouseDelta(int* x, int* y);

        /*********************************************************************************//*!
        \brief      Determine if any supported mouse button has been triggered during this frame

        \note       Mainly used for C# side calling

        \return     true if any supported mouse button has been triggered during this frame, else false
        *//**********************************************************************************/
        SCRIPT_API bool IsAnyMouseButtonPressed();

        /*********************************************************************************//*!
        \brief      Determine if any supported mouse button is currently held down

        \note       Mainly used for C# side calling

        \return     true if any supported mouse button is currently held down, else false
        *//**********************************************************************************/
        SCRIPT_API bool IsAnyMouseButtonHeld();

        /*********************************************************************************//*!
        \brief      Determine if any supported mouse button has been released during this frame

        \note       Mainly used for C# side calling

        \return     true if any supported mouse button has been released during this frame else false
        *//**********************************************************************************/
        SCRIPT_API bool IsAnyMouseButtonReleased();

        /*********************************************************************************//*!
        \brief      Determine if a supported mouse button by the given mousecode has been triggered during this frame

        \note       Mainly used for C# side calling

        \return     true if the specified mouse button has been triggered during this frame, else false
        *//**********************************************************************************/
        SCRIPT_API bool IsMouseButtonPressed(int button);

        /*********************************************************************************//*!
        \brief      Determine if a supported mouse button by the given mousecode is currently held down

        \note       Mainly used for C# side calling

        \return     true if the specified mouse button is currently held down, else false
        *//**********************************************************************************/
        SCRIPT_API bool IsMouseButtonHeld(int button);

        /*********************************************************************************//*!
        \brief      Determine if a supported mouse button by the given mousecode has been triggered during this frame

        \note       Mainly used for C# side calling

        \return     true if the specified mouse button has been triggered during this frame, else false
        *//**********************************************************************************/
        SCRIPT_API bool IsMouseButtonReleased(int button);
    }
}
