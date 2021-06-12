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
    typedef enum class KeyCode
    {
        A               = 4,
        B               = 5,
        C               = 6,
        D               = 7,
        E               = 8,
        F               = 9,
        G               = 10,
        H               = 11,
        I               = 12,
        J               = 13,
        K               = 14,
        L               = 15,
        M               = 16,
        N               = 17,
        O               = 18,
        P               = 19,
        Q               = 20,
        R               = 21,
        S               = 22,
        T               = 23,
        U               = 24,
        V               = 25,
        W               = 26,
        X               = 27,
        Y               = 28,
        Z               = 29,

        D1              = 30,
        D2              = 31,
        D3              = 32,
        D4              = 33,
        D5              = 34,
        D6              = 35,
        D7              = 36,
        D8              = 37,
        D9              = 38,
        D0              = 39,
            
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
        DEL             = 76,/* DELETE <- writing this will conflict with some header defines*/
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
        RGUI            = 231, /**< windows, command (apple), meta */
    } Key;

    inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
    {
        os << static_cast<int>(keyCode);
        return os;
    }

    inline KeyCode& operator++(KeyCode& keyCode)
    {
        keyCode = static_cast<KeyCode>(static_cast<int>(keyCode) + 1);
        return keyCode;
    }

    inline bool operator<(KeyCode keyCode, int val)
    {
        return static_cast<int>(keyCode) < val;
    }

}


// From SDL keycodes

//#define ENGINE_KEY_SPACE           ::engine::Key::Space
//#define ENGINE_KEY_APOSTROPHE      ::engine::Key::Apostrophe    /* ' */
//#define ENGINE_KEY_COMMA           ::engine::Key::Comma         /* , */
//#define ENGINE_KEY_MINUS           ::engine::Key::Minus         /* - */
//#define ENGINE_KEY_PERIOD          ::engine::Key::Period        /* . */
//#define ENGINE_KEY_SLASH           ::engine::Key::Slash         /* / */
#define ENGINE_KEY_0               ::engine::Key::D0
#define ENGINE_KEY_1               ::engine::Key::D1
#define ENGINE_KEY_2               ::engine::Key::D2
#define ENGINE_KEY_3               ::engine::Key::D3
#define ENGINE_KEY_4               ::engine::Key::D4
#define ENGINE_KEY_5               ::engine::Key::D5
#define ENGINE_KEY_6               ::engine::Key::D6
#define ENGINE_KEY_7               ::engine::Key::D7
#define ENGINE_KEY_8               ::engine::Key::D8
#define ENGINE_KEY_9               ::engine::Key::D9
//#define ENGINE_KEY_SEMICOLON       ::engine::Key::Semicolon     /* ; */
//#define ENGINE_KEY_EQUAL           ::engine::Key::Equal         /* = */
#define ENGINE_KEY_A               ::engine::Key::A
#define ENGINE_KEY_B               ::engine::Key::B
#define ENGINE_KEY_C               ::engine::Key::C
#define ENGINE_KEY_D               ::engine::Key::D
#define ENGINE_KEY_E               ::engine::Key::E
#define ENGINE_KEY_F               ::engine::Key::F
#define ENGINE_KEY_G               ::engine::Key::G
#define ENGINE_KEY_H               ::engine::Key::H
#define ENGINE_KEY_I               ::engine::Key::I
#define ENGINE_KEY_J               ::engine::Key::J
#define ENGINE_KEY_K               ::engine::Key::K
#define ENGINE_KEY_L               ::engine::Key::L
#define ENGINE_KEY_M               ::engine::Key::M
#define ENGINE_KEY_N               ::engine::Key::N
#define ENGINE_KEY_O               ::engine::Key::O
#define ENGINE_KEY_P               ::engine::Key::P
#define ENGINE_KEY_Q               ::engine::Key::Q
#define ENGINE_KEY_R               ::engine::Key::R
#define ENGINE_KEY_S               ::engine::Key::S
#define ENGINE_KEY_T               ::engine::Key::T
#define ENGINE_KEY_U               ::engine::Key::U
#define ENGINE_KEY_V               ::engine::Key::V
#define ENGINE_KEY_W               ::engine::Key::W
#define ENGINE_KEY_X               ::engine::Key::X
#define ENGINE_KEY_Y               ::engine::Key::Y
#define ENGINE_KEY_Z               ::engine::Key::Z
//#define ENGINE_KEY_LEFT_BRACKET    ::engine::Key::LeftBracket   /* [ */
//#define ENGINE_KEY_BACKSLASH       ::engine::Key::Backslash     /* \ */
//#define ENGINE_KEY_RIGHT_BRACKET   ::engine::Key::RightBracket  /* ] */
//#define ENGINE_KEY_GRAVE_ACCENT    ::engine::Key::GraveAccent   /* ` */
//#define ENGINE_KEY_WORLD_1         ::engine::Key::World1        /* non-US #1 */
//#define ENGINE_KEY_WORLD_2         ::engine::Key::World2        /* non-US #2 */

/* Function keys */
//#define ENGINE_KEY_ESCAPE          ::engine::Key::Escape
//#define ENGINE_KEY_ENTER           ::engine::Key::Enter
//#define ENGINE_KEY_TAB             ::engine::Key::Tab
//#define ENGINE_KEY_BACKSPACE       ::engine::Key::Backspace
//#define ENGINE_KEY_INSERT          ::engine::Key::Insert
//#define ENGINE_KEY_DELETE          ::engine::Key::Delete
//#define ENGINE_KEY_RIGHT           ::engine::Key::Right
//#define ENGINE_KEY_LEFT            ::engine::Key::Left
//#define ENGINE_KEY_DOWN            ::engine::Key::Down
//#define ENGINE_KEY_UP              ::engine::Key::Up
//#define ENGINE_KEY_PAGE_UP         ::engine::Key::PageUp
//#define ENGINE_KEY_PAGE_DOWN       ::engine::Key::PageDown
//#define ENGINE_KEY_HOME            ::engine::Key::Home
//#define ENGINE_KEY_END             ::engine::Key::End
//#define ENGINE_KEY_CAPS_LOCK       ::engine::Key::CapsLock
//#define ENGINE_KEY_SCROLL_LOCK     ::engine::Key::ScrollLock
//#define ENGINE_KEY_NUM_LOCK        ::engine::Key::NumLock
//#define ENGINE_KEY_PRINT_SCREEN    ::engine::Key::PrintScreen
//#define ENGINE_KEY_PAUSE           ::engine::Key::Pause
#define ENGINE_KEY_F1              ::engine::Key::F1
#define ENGINE_KEY_F2              ::engine::Key::F2
#define ENGINE_KEY_F3              ::engine::Key::F3
#define ENGINE_KEY_F4              ::engine::Key::F4
#define ENGINE_KEY_F5              ::engine::Key::F5
#define ENGINE_KEY_F6              ::engine::Key::F6
#define ENGINE_KEY_F7              ::engine::Key::F7
#define ENGINE_KEY_F8              ::engine::Key::F8
#define ENGINE_KEY_F9              ::engine::Key::F9
#define ENGINE_KEY_F10             ::engine::Key::F10
#define ENGINE_KEY_F11             ::engine::Key::F11
#define ENGINE_KEY_F12             ::engine::Key::F12
//#define ENGINE_KEY_F13             ::engine::Key::F13
//#define ENGINE_KEY_F14             ::engine::Key::F14
//#define ENGINE_KEY_F15             ::engine::Key::F15
//#define ENGINE_KEY_F16             ::engine::Key::F16
//#define ENGINE_KEY_F17             ::engine::Key::F17
//#define ENGINE_KEY_F18             ::engine::Key::F18
//#define ENGINE_KEY_F19             ::engine::Key::F19
//#define ENGINE_KEY_F20             ::engine::Key::F20
//#define ENGINE_KEY_F21             ::engine::Key::F21
//#define ENGINE_KEY_F22             ::engine::Key::F22
//#define ENGINE_KEY_F23             ::engine::Key::F23
//#define ENGINE_KEY_F24             ::engine::Key::F24
//#define ENGINE_KEY_F25             ::engine::Key::F25

/* Keypad */
#define ENGINE_KEY_KP_0            ::engine::Key::KP_0
#define ENGINE_KEY_KP_1            ::engine::Key::KP_1
#define ENGINE_KEY_KP_2            ::engine::Key::KP_2
#define ENGINE_KEY_KP_3            ::engine::Key::KP_3
#define ENGINE_KEY_KP_4            ::engine::Key::KP_4
#define ENGINE_KEY_KP_5            ::engine::Key::KP_5
#define ENGINE_KEY_KP_6            ::engine::Key::KP_6
#define ENGINE_KEY_KP_7            ::engine::Key::KP_7
#define ENGINE_KEY_KP_8            ::engine::Key::KP_8
#define ENGINE_KEY_KP_9            ::engine::Key::KP_9
//#define ENGINE_KEY_KP_DECIMAL      ::engine::Key::KPDecimal
//#define ENGINE_KEY_KP_DIVIDE       ::engine::Key::KPDivide
//#define ENGINE_KEY_KP_MULTIPLY     ::engine::Key::KPMultiply
//#define ENGINE_KEY_KP_SUBTRACT     ::engine::Key::KPSubtract
//#define ENGINE_KEY_KP_ADD          ::engine::Key::KPAdd
//#define ENGINE_KEY_KP_ENTER        ::engine::Key::KPEnter
//#define ENGINE_KEY_KP_EQUAL        ::engine::Key::KPEqual

//#define ENGINE_KEY_LEFT_SHIFT      ::engine::Key::LeftShift
//#define ENGINE_KEY_LEFT_CONTROL    ::engine::Key::LeftControl
//#define ENGINE_KEY_LEFT_ALT        ::engine::Key::LeftAlt
//#define ENGINE_KEY_LEFT_SUPER      ::engine::Key::LeftSuper
//#define ENGINE_KEY_RIGHT_SHIFT     ::engine::Key::RightShift
//#define ENGINE_KEY_RIGHT_CONTROL   ::engine::Key::RightControl
//#define ENGINE_KEY_RIGHT_ALT       ::engine::Key::RightAlt
//#define ENGINE_KEY_RIGHT_SUPER     ::engine::Key::RightSuper
//#define ENGINE_KEY_MENU            ::engine::Key::Menu
