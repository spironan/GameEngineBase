/************************************************************************************//*!
\file           KeyCode.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 24, 2021
\brief          Defines the engine keycodes that are used the input system to map each
                key to a behaviour regardless of what the backend is using.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

namespace engine
{
    using KeyCode = int;

    namespace key
    {
        enum : KeyCode
        {
            /* from SDL2 */
            A = 4,
            B = 5,
            C = 6,
            D = 7,
            E = 8,
            F = 9,
            G = 10,
            H = 11,
            I = 12,
            J = 13,
            K = 14,
            L = 15,
            M = 16,
            N = 17,
            O = 18,
            P = 19,
            Q = 20,
            R = 21,
            S = 22,
            T = 23,
            U = 24,
            V = 25,
            W = 26,
            X = 27,
            Y = 28,
            Z = 29,

            D1 = 30,
            D2 = 31,
            D3 = 32,
            D4 = 33,
            D5 = 34,
            D6 = 35,
            D7 = 36,
            D8 = 37,
            D9 = 38,
            D0 = 39,
            
            RETURN          = 40,
            ESCAPE          = 41,
            BACKSPACE       = 42,
            TAB             = 43,
            SPACE           = 44,

            MINUS           = 45,
            EQUALS          = 46,
            LEFTBRACKET     = 47,
            RIGHTBRACKET    = 48,
            BACKSLASH       = 49,       /**< Located at the lower left of the return
                                        *   key on ISO keyboards and at the right end
                                        *   of the QWERTY row on ANSI keyboards.
                                        *   Produces REVERSE SOLIDUS (backslash) and
                                        *   VERTICAL LINE in a US layout, REVERSE
                                        *   SOLIDUS and VERTICAL LINE in a UK Mac
                                        *   layout, NUMBER SIGN and TILDE in a UK
                                        *   Windows layout, DOLLAR SIGN and POUND SIGN
                                        *   in a Swiss German layout, NUMBER SIGN and
                                        *   APOSTROPHE in a German layout, GRAVE
                                        *   ACCENT and POUND SIGN in a French Mac
                                        *   layout, and ASTERISK and MICRO SIGN in a
                                        *   French Windows layout.
                                        */
            NONUSHASH       = 50,       /**< ISO USB keyboards actually use this code
                                        *   instead of 49 for the same key, but all
                                        *   OSes I've seen treat the two codes
                                        *   identically. So, as an implementor, unless
                                        *   your keyboard generates both of those
                                        *   codes and your OS treats them differently,
                                        *   you should generate SDL_SCANCODE_BACKSLASH
                                        *   instead of this code. As a user, you
                                        *   should not rely on this code because SDL
                                        *   will never generate it with most (all?)
                                        *   keyboards.
                                        */
            SEMICOLON       = 51,
            APOSTROPHE      = 52,
            GRAVE           = 53,       /**< Located in the top left corner (on both ANSI
                                        *   and ISO keyboards). Produces GRAVE ACCENT and
                                        *   TILDE in a US Windows layout and in US and UK
                                        *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                                        *   and NOT SIGN in a UK Windows layout, SECTION
                                        *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                                        *   layouts on ISO keyboards, SECTION SIGN and
                                        *   DEGREE SIGN in a Swiss German layout (Mac:
                                        *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                                        *   DEGREE SIGN in a German layout (Mac: only on
                                        *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                                        *   French Windows layout, COMMERCIAL AT and
                                        *   NUMBER SIGN in a French Mac layout on ISO
                                        *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                                        *   SIGN in a Swiss German, German, or French Mac
                                        *   layout on ANSI keyboards.
                                        */
            COMMA           = 54,
            PERIOD          = 55,
            SLASH           = 56,

            CAPSLOCK        = 57,

            F1              = 58,
            F2              = 59,
            F3              = 60,
            F4              = 61,
            F5              = 62,
            F6              = 63,
            F7              = 64,
            F8              = 65,
            F9              = 66,
            F10             = 67,
            F11             = 68,
            F12             = 69,

            PRINTSCREEN     = 70,
            SCROLLLOCK      = 71,
            PAUSE           = 72,
            INSERT          = 73, /**< insert on PC, help on some Mac keyboards (but does send code 73, not 117) */
            HOME            = 74,
            PAGEUP          = 75,
            //DELETE          = 76,
            END             = 77,
            PAGEDOWN        = 78,
            RIGHT           = 79,
            LEFT            = 80,
            DOWN            = 81,
            UP              = 82,

            NUMLOCKCLEAR    = 83, /**< num lock on PC, clear on Mac keyboards*/
            KP_DIVIDE       = 84,
            KP_MULTIPLY     = 85,
            KP_MINUS        = 86,
            KP_PLUS         = 87,
            KP_ENTER        = 88,
            KP_1            = 89,
            KP_2            = 90,
            KP_3            = 91,
            KP_4            = 92,
            KP_5            = 93,
            KP_6            = 94,
            KP_7            = 95,
            KP_8            = 96,
            KP_9            = 97,
            KP_0            = 98,
            KP_PERIOD       = 99,

            LCTRL           = 224,
            LSHIFT          = 225,
            LALT            = 226, /**< alt, option */
            LGUI            = 227, /**< windows, command (apple), meta */
            RCTRL           = 228,
            RSHIFT          = 229,
            RALT            = 230, /**< alt gr, option */
            RGUI            = 231 /**< windows, command (apple), meta */



                // From glfw3.h
                //Space = 32,
                //Apostrophe = 39, /* ' */
                //Comma = 44, /* , */
                //Minus = 45, /* - */
                //Period = 46, /* . */
                //Slash = 47, /* / */

                //D0 = 48, /* 0 */
                //D1 = 49, /* 1 */
                //D2 = 50, /* 2 */
                //D3 = 51, /* 3 */
                //D4 = 52, /* 4 */
                //D5 = 53, /* 5 */
                //D6 = 54, /* 6 */
                //D7 = 55, /* 7 */
                //D8 = 56, /* 8 */
                //D9 = 57, /* 9 */

                //Semicolon = 59, /* ; */
                //Equal = 61, /* = */

                //A = 65,
                //B = 66,
                //C = 67,
                //D = 68,
                //E = 69,
                //F = 70,
                //G = 71,
                //H = 72,
                //I = 73,
                //J = 74,
                //K = 75,
                //L = 76,
                //M = 77,
                //N = 78,
                //O = 79,
                //P = 80,
                //Q = 81,
                //R = 82,
                //S = 83,
                //T = 84,
                //U = 85,
                //V = 86,
                //W = 87,
                //X = 88,
                //Y = 89,
                //Z = 90,

                //LeftBracket = 91,  /* [ */
                //Backslash = 92,  /* \ */
                //RightBracket = 93,  /* ] */
                //GraveAccent = 96,  /* ` */

                //World1 = 161, /* non-US #1 */
                //World2 = 162, /* non-US #2 */

                ///* Function keys */
                //Escape = 256,
                //Enter = 257,
                //Tab = 258,
                //Backspace = 259,
                //Insert = 260,
                //Delete = 261,
                //Right = 262,
                //Left = 263,
                //Down = 264,
                //Up = 265,
                //PageUp = 266,
                //PageDown = 267,
                //Home = 268,
                //End = 269,
                //CapsLock = 280,
                //ScrollLock = 281,
                //NumLock = 282,
                //PrintScreen = 283,
                //Pause = 284,
                //F1 = 290,
                //F2 = 291,
                //F3 = 292,
                //F4 = 293,
                //F5 = 294,
                //F6 = 295,
                //F7 = 296,
                //F8 = 297,
                //F9 = 298,
                //F10 = 299,
                //F11 = 300,
                //F12 = 301,
                //F13 = 302,
                //F14 = 303,
                //F15 = 304,
                //F16 = 305,
                //F17 = 306,
                //F18 = 307,
                //F19 = 308,
                //F20 = 309,
                //F21 = 310,
                //F22 = 311,
                //F23 = 312,
                //F24 = 313,
                //F25 = 314,

                ///* Keypad */
                //KP0 = 320,
                //KP1 = 321,
                //KP2 = 322,
                //KP3 = 323,
                //KP4 = 324,
                //KP5 = 325,
                //KP6 = 326,
                //KP7 = 327,
                //KP8 = 328,
                //KP9 = 329,
                //KPDecimal = 330,
                //KPDivide = 331,
                //KPMultiply = 332,
                //KPSubtract = 333,
                //KPAdd = 334,
                //KPEnter = 335,
                //KPEqual = 336,

                //LeftShift = 340,
                //LeftControl = 341,
                //LeftAlt = 342,
                //LeftSuper = 343,
                //RightShift = 344,
                //RightControl = 345,
                //RightAlt = 346,
                //RightSuper = 347,
                //Menu = 348
            
        };
    }
}