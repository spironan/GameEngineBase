/************************************************************************************//*!
\file           ScriptUtility.h
\project        <PROJECT_NAME>
\author         Solomon Tan Teng Shue, t.tengshuesolomon, 620010020
\par            email: t.tengshuesolomon\@digipen.edu
\date           August 3, 2021
\brief          Defines and contains the necessary data and functions needed to
                make use of mono. This header file should not be included in any
                other header files that end up included in any project other than Engine

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/

#pragma once

#include "ScriptInfo.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/threads.h>

namespace engine
{
    namespace ScriptUtility
    {
        struct ScriptSystemInfo
        {
            MonoDomain* domain;
            MonoAssembly* assembly;
            MonoImage* image;
            std::vector<ScriptClassInfo> classInfoList;

            inline ScriptSystemInfo() : domain{ nullptr }, assembly{ nullptr }, image{ nullptr } {};

            /*********************************************************************************//*!
            \brief      Resets all current system info, usually used when recompiling
            *//**********************************************************************************/
            inline void Reset()
            {
                domain = nullptr;
                assembly = nullptr;
                image = nullptr;
                classInfoList.clear();
            }

            /*********************************************************************************//*!
            \brief      Checks if the necessary system information has been set up yet
             
            \return     true if it has been set up and is ready for use, else false
            *//**********************************************************************************/
            inline bool IsSetUp() { return image != nullptr; }
        };

        extern ScriptSystemInfo g_SystemInfo;

        
        /*********************************************************************************//*!
        \brief      returns a pointer to the corresponding MonoClass
                    with the given namespace and class name
         
        \param      name_space
                the namespace of the desired C# class (nested namespaces separated by '.')
                    name
                the name of the desired C# class (nested classes won't work)

        \return     the pointer to the desired MonoClass, null if not found
        
        *//**********************************************************************************/
        MonoClass* GetMonoClass(const char* name_space, const char* name);

        /*********************************************************************************//*!
        \brief      returns a pointer to the corresponding MonoClass
                    with the given class info
         
        \param      classInfo
                the ScriptClassInfo containing the namespace and name of the desired C# class

        \return     the pointer to the desired MonoClass, null if not found
        
        *//**********************************************************************************/
        MonoClass* GetMonoClass(ScriptClassInfo const& classInfo);

        /*********************************************************************************//*!
        \brief      Helper function to get the pointer to the C# GameObject class

        \return     the pointer to the C# GameObject MonoClass
        *//**********************************************************************************/
        MonoClass* GetGameObjectMonoClass();

        /*********************************************************************************//*!
        \brief      Helper function to get the pointer to the C# Component class,
                    which both scripts and ECS component C# interfaces will inherit from

        \return     the pointer to the C# Component MonoClass
        *//**********************************************************************************/
        MonoClass* GetBaseComponentMonoClass();

        /*********************************************************************************//*!
        \brief      Helper function to get the pointer to the C# MonoBehaviour class,
                    which all scripts inherit from

        \return     the pointer to the C# MonoBehaviour MonoClass
        *//**********************************************************************************/
        MonoClass* GetBaseScriptMonoClass();

        /*********************************************************************************//*!
        \brief      Helper function to create a new instance of a given class in C# side
         
        \param      _class
                the pointer to the MonoClass of the new object

        \return     the pointer to the newly created MonoObject (C# object)
        *//**********************************************************************************/
        MonoObject* MonoObjectNew(MonoClass* _class);

        /*********************************************************************************//*!
        \brief      Helper function to create a new string in C# side

        \param      text
                the desired text of the C# string

        \return     the pointer to the newly created MonoString (C# string)
        *//**********************************************************************************/
        MonoString* MonoStringNew(const char* text);

        /*********************************************************************************//*!
        \brief      Helper function to check if a given C# class field is marked public

        \param      field
                the pointer to the MonoClassField to check

        \return     true if the given field is public, else false
        *//**********************************************************************************/
        bool IsClassFieldPublic(MonoClassField* field);

        /*********************************************************************************//*!
        \brief      Helper function to check if a given C# class inherits from another C# class

        \param      _class
                the pointer to the MonoClass to check
                    desiredBase
                the pointer to the desired base MonoClass that _class may inherit from

        \return     true if the given class inherits from the given base class, else false
        *//**********************************************************************************/
        bool CheckBaseClass(MonoClass* _class, MonoClass* desiredBase);

        /*********************************************************************************//*!
        \brief      Helper function to get a function from a given C# object's class by name, if any

        \param      obj
                the pointer to the MonoObject to get the function from
                    functionName
                the name of the desired function

        \return     the pointer to the C# function found, or null if none were found
        *//**********************************************************************************/
        MonoMethod* FindFunction(MonoObject* obj, const char* functionName);

        /*********************************************************************************//*!
        \brief      Helper function to get a list of C# classes used by a generic C# type

        \param      type
                the pointer to the MonoType to get the list of C# classes from

        \return     a vector of pointers to the MonoClasses used by the generic C# type
        *//**********************************************************************************/
        std::vector<MonoClass*> const GetMonoTypeGenericClasses(MonoType* type);
    }
}
