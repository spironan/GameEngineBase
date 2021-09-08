/************************************************************************************//*!
\file           Input.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 24, 2021
\brief          Implementation of the general abstracted input system that will be used
                by the client.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/MouseCode.h"
#include "Engine/Core/KeyCode.h"

namespace engine
{
    // forward declaration
    class Application;

    /****************************************************************************//*!
     @brief     Interface of the Engine's Input system that will be used by 
                by the client.
     
     @note      Check out KeyCode and MouseCode for more information on supported
                keys.
    *//*****************************************************************************/
    class Input
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Interface Functions                                                         */
        /*-----------------------------------------------------------------------------*/
        /****************************************************************************//*!
         @brief     Determine if a key by the given keycode is currently held down

         @note      This function will return true even when keypressed is true too.

         @return    Whether the key is currently down.
        *//*****************************************************************************/
        static bool IsKeyHeld(KeyCode keycode);
        /****************************************************************************//*!
         @brief     Determine if a key by the given keycode has been triggered within
                    this frame.

         @return    Whether the key is triggered this frame .
        *//*****************************************************************************/
        static bool IsKeyPressed(KeyCode keycode);
        /****************************************************************************//*!
         @brief     Determine if a key by the given keycode has been triggered within
                    this frame.
         
         @note      KeyRelease returns true once when a key that was pressed is released.
                    To check if a key is not pressed all the time 
                    use !(IsKeyDown(keycode)) instead.

         @return    Whether the key is Released this frame.
        *//*****************************************************************************/
        static bool IsKeyReleased(KeyCode keycode);
        /****************************************************************************//*!
         @brief     Determine if any supported key is currently held down

         @note      This function will return true even when keypressed is true too.

         @return    Whether any key is pressed down
        *//*****************************************************************************/
        static bool IsAnyKeyHeld();
        /****************************************************************************//*!
         @brief     Determine if any supported key has been triggered within
                    this frame.

         @return    Whether any key is triggered this frame
        *//*****************************************************************************/
        static bool IsAnyKeyPressed();
        /****************************************************************************//*!
         @brief     Determine if any supported key by the given keycode has 
                    been triggered within this frame.

         @note      KeyRelease returns true once when a key that was pressed is released.

         @return    Whether any key is Released this frame.
        *//*****************************************************************************/
        static bool IsAnyKeyReleased();
        /****************************************************************************//*!
         @brief     Retrieve a vector of supported keys that are currently pressed down.

         @return    Returns a vector to all supported keys that is released 
        *//*****************************************************************************/
        static std::vector<KeyCode> GetKeysHeld();
        /****************************************************************************//*!
         @brief     Retrieve a vector of supported keys that are currently pressed.

         @return    Returns a vector to all supported keys that is pressed down this 
                    frame
        *//*****************************************************************************/
        static std::vector<KeyCode> GetKeysPressed();
        /****************************************************************************//*!
         @brief     Retrieve a vector of supported keys that are currently released.

         @return    Returns a vector of supported keys that is released this frame.
        *//*****************************************************************************/
        static std::vector<KeyCode> GetKeysReleased();

        /****************************************************************************//*!
         @brief     Determine if a mouse button by the given mouse code is 
                    currently held down

         @note      This function will return true even when mousepressed is true too.

         @return    Whether the mouse button is currently down.
        *//*****************************************************************************/
        static bool IsMouseButtonHeld(MouseCode button);
        /****************************************************************************//*!
         @brief     Determine if a mouse button by the given mouse code is
                    has been triggered this frame.

         @return    Whether the mouse button is being clicked this frame.
        *//*****************************************************************************/
        static bool IsMouseButtonPressed(MouseCode button);
        /****************************************************************************//*!
         @brief     Determine if a mouse button by the given mouse code is
                    has been released this frame.

         @return    Whether the mouse button is being released this frame.
        *//*****************************************************************************/
        static bool IsMouseButtonReleased(MouseCode button);
        /****************************************************************************//*!
         @brief     Determine if any supported mouse button is currently held down

         @note      This function will return true even when mousepressed is true too.

         @return    Whether any supported mouse button is currently down.
        *//*****************************************************************************/
        static bool IsAnyMouseButtonHeld();
        /****************************************************************************//*!
         @brief     Determine if any supported mouse button is has 
                    been triggered this frame.

         @return    Whether any supported mouse button has been clicked this frame.
        *//*****************************************************************************/
        static bool IsAnyMouseButtonPressed();
        /****************************************************************************//*!
         @brief     Determine if any supported mouse button is has
                    been triggered this frame.

         @return    Whether any supported mouse button has been clicked this frame.
        *//*****************************************************************************/
        static bool IsAnyMouseButtonReleased();
        /****************************************************************************//*!
         @brief     Retrieve all supported mouse button that is currently held down

         @return    Return a vector to all supported mouse buttons that are currently
                    held down.
        *//*****************************************************************************/
        static std::vector<MouseCode> GetMouseButtonsHeld();
        /****************************************************************************//*!
         @brief     Retrieve all supported mouse button that is pressed this frame

         @return    Return a vector to all supported mouse buttons that is currently
                    pressed this frame.
        *//*****************************************************************************/
        static std::vector<MouseCode> GetMouseButtonsPressed();
        /****************************************************************************//*!
         @brief     Retrieve all supported mouse button that is are released this frame

         @return    Return a vector to all supported mouse buttons that are released
                    this frame.
        *//*****************************************************************************/
        static std::vector<MouseCode> GetMouseButtonsReleased();


        /****************************************************************************//*!
         @brief     Retrieve the current mouse position in screen coordinates 
                    (top left 0,0)

         @return    a pair containing the x and y position of the mouse
        *//*****************************************************************************/
        static std::pair<int, int> GetMousePosition();
        
        /****************************************************************************//*!
         @brief     Retrieve the change in mouse position in the current frame 
                    in screen coordinates (top left 0,0)

         @return    a pair containing the x and y delta position of the mouse
        *//*****************************************************************************/
        static std::pair<int, int> GetMouseDelta();

        /****************************************************************************//*!
         @brief     Retrieve the current X-axis mouse position in screen coordinates

         @return    X-axis position of where the mouse current is at
        *//*****************************************************************************/
        static int GetMouseX();
        /****************************************************************************//*!
         @brief     Retrieve the current Y-axis mouse position in screen coordinates

         @return    Y-axis position of where the mouse current is at
        *//*****************************************************************************/
        static int GetMouseY();
    
    private:
        Input() = delete;
        ~Input() = delete;

        static void Init();
        static void Update();
        static void ShutDown();

        friend class Application;
    };
}