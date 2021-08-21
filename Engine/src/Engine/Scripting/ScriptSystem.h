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
#include "Scripting.h"

namespace engine
{
    class ScriptSystem final : public System
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors & Destructor                                                   */
        /*-----------------------------------------------------------------------------*/
        explicit ScriptSystem(ECS_Manager& _ECS_Manager) : System{ _ECS_Manager }, isPlaying{ false } {};
        ~ScriptSystem();

        /*-----------------------------------------------------------------------------*/
        /* Compiling                                                                   */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      executes the required actions to compile all user made scripts during runtime
        *//**********************************************************************************/
        void Compile();

        /*********************************************************************************//*!
        \brief      Checks if the C# scripts have been properly compiled and the script system
                    is properly set up and ready for use

        \return     true if it has been set up and is ready for use, else false
        *//**********************************************************************************/
        bool IsSetUp();

        /*-----------------------------------------------------------------------------*/
        /* Mode Functions                                                              */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      executes the necessary functions in all Scripting components to initiate play mode
        *//**********************************************************************************/
        void StartPlay();

        /*********************************************************************************//*!
        \brief      executes the necessary functions in all Scripting components to stop play mode
        *//**********************************************************************************/
        void StopPlay();

        /*-----------------------------------------------------------------------------*/
        /* Function Invoking                                                           */
        /*-----------------------------------------------------------------------------*/
        
        /*********************************************************************************//*!
        \brief      invokes a function by name in all script instances of all GameObjects
         
        \param      functionName
                the name of the desired function
        *//**********************************************************************************/
        void InvokeFunctionAll(const char* functionName);

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
        bool isPlaying;
    };

    extern "C"
    {
        /*-----------------------------------------------------------------------------*/
        /* Entity Functions for C#                                                     */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      Creates a new GameObject in the currently active world with a Scripting component
                    in play mode attached.
                    This function's main purpose is to enable scripts in the C# side to create entities
                    during play mode

        \warning    this function should only be called during play mode as instances in C# side
                    are created in this function, which will break if a recompile is triggered

        \return     the IntPtr to the newly created GameObject in C# side
        *//**********************************************************************************/
        __declspec(dllexport) uint32_t CreateEntity();

        /*********************************************************************************//*!
        \brief      Removes a GameObject in the currently active world with a specific entity id.
                    This function's main purpose is to enable scripts in the C# side to remove entities
                    during play mode

        \param      id
                the entity id of the GameObject to be destroyed
        *//**********************************************************************************/
        __declspec(dllexport) void DestroyEntity(int id);

        /*-----------------------------------------------------------------------------*/
        /* Input Functions for C#                                                      */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      Determine if any supported key is currently held down.
                    
        \note       Mainly used for C# side calling

        \return     true if any supported key is currently held down, else false
        *//**********************************************************************************/
        __declspec(dllexport) bool AnyKey();

        /*********************************************************************************//*!
        \brief      Determine if any supported key has been triggered during this frame

        \note       Mainly used for C# side calling

        \return     true if any supported key was triggered during this frame, else false
        *//**********************************************************************************/
        __declspec(dllexport) bool AnyKeyDown();

        /*********************************************************************************//*!
        \brief      Determine if any supported key has been released during this frame

        \note       Mainly used for C# side calling

        \return     true if any supported key was released during this frame, else false
        *//**********************************************************************************/
        __declspec(dllexport) bool AnyKeyUp();

        /*********************************************************************************//*!
        \brief      Determine if a supported key by the given keycode is currently held down

        \note       Mainly used for C# side calling

        \return     true if the specified key is currently held down, else false
        *//**********************************************************************************/
        __declspec(dllexport) bool GetKey(int key);

        /*********************************************************************************//*!
        \brief      Determine if a supported key by the given keycode has been triggered during this frame

        \note       Mainly used for C# side calling

        \return     true if the specified key was triggered during this frame, else false
        *//**********************************************************************************/
        __declspec(dllexport) bool GetKeyDown(int key);

        /*********************************************************************************//*!
        \brief      Determine if a supported key by the given keycode has been released during this frame

        \note       Mainly used for C# side calling

        \return     true if the specified key was released during this frame, else false
        *//**********************************************************************************/
        __declspec(dllexport) bool GetKeyUp(int key);

        /*********************************************************************************//*!
        \brief      Retrieves the current mouse position in screen coordinates
                    (top left 0,0)

        \note       Mainly used for C# side calling

        \param      x
                the pointer to set the x-coordinate of the mouse screen position to

        \param      y
                the pointer to set the y-coordinate of the mouse screen position to
        *//**********************************************************************************/
        __declspec(dllexport) void GetMousePosition(int* x, int* y);

        /*********************************************************************************//*!
        \brief      Retrieves the change in mouse position in the current frame in screen coordinates
                    (top left 0,0)

        \note       Mainly used for C# side calling

        \param      x
                the pointer to set the x-coordinate of the mouse screen position change to

        \param      y
                the pointer to set the y-coordinate of the mouse screen position change to
        *//**********************************************************************************/
        __declspec(dllexport) void GetMouseDelta(int* x, int* y);

        /*********************************************************************************//*!
        \brief      Determine if any supported mouse button is currently held down

        \note       Mainly used for C# side calling

        \return     true if any supported mouse button is currently held down, else false
        *//**********************************************************************************/
        __declspec(dllexport) bool AnyMouseButton();

        /*********************************************************************************//*!
        \brief      Determine if any supported mouse button has been triggered during this frame

        \note       Mainly used for C# side calling

        \return     true if any supported mouse button has been triggered during this frame, else false
        *//**********************************************************************************/
        __declspec(dllexport) bool AnyMouseButtonDown();

        /*********************************************************************************//*!
        \brief      Determine if any supported mouse button has been released during this frame

        \note       Mainly used for C# side calling

        \return     true if any supported mouse button has been released during this frame else false
        *//**********************************************************************************/
        __declspec(dllexport) bool AnyMouseButtonUp();

        /*********************************************************************************//*!
        \brief      Determine if a supported mouse button by the given mousecode is currently held down

        \note       Mainly used for C# side calling

        \return     true if the specified mouse button is currently held down, else false
        *//**********************************************************************************/
        __declspec(dllexport) bool GetMouseButton(int button);

        /*********************************************************************************//*!
        \brief      Determine if a supported mouse button by the given mousecode has been triggered during this frame

        \note       Mainly used for C# side calling

        \return     true if the specified mouse button has been triggered during this frame, else false
        *//**********************************************************************************/
        __declspec(dllexport) bool GetMouseButtonDown(int button);

        /*********************************************************************************//*!
        \brief      Determine if a supported mouse button by the given mousecode has been triggered during this frame

        \note       Mainly used for C# side calling

        \return     true if the specified mouse button has been triggered during this frame, else false
        *//**********************************************************************************/
        __declspec(dllexport) bool GetMouseButtonUp(int button);
    }
}