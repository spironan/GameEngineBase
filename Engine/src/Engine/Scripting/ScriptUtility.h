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
#include "ScriptSystem.h"

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
            MonoImage* scripting;
            std::vector<ScriptClassInfo> classInfoList;
            std::unordered_map<MonoType*, ScriptSystem::RegisteredComponent> componentMap;
            std::unordered_map<MonoType*, size_t> componentIDMap;

            inline ScriptSystemInfo() : domain{ nullptr }, scripting{ nullptr } {};

            /*********************************************************************************//*!
            \brief      initializes all script system info using the dll at the indicated path for later use
             
            \param      dllPath
                    the file path to the scripting dll
            
            *//**********************************************************************************/
            void Initialize(const char* dllPath, std::unordered_map<std::string, ScriptSystem::RegisteredComponent> const& refMap);

            /*********************************************************************************//*!
            \brief      Resets all current system info, usually used to enable recompiling
            *//**********************************************************************************/
            void Reset();

            /*********************************************************************************//*!
            \brief      Checks if the necessary system information has been set up yet
             
            \return     true if it has been set up and is ready for use, else false
            *//**********************************************************************************/
            inline bool IsSetUp() { return scripting != nullptr; }
        };

        extern ScriptSystemInfo g_SystemInfo;

        
        /*********************************************************************************//*!
        \brief      returns a pointer to the corresponding MonoClass
                    with the given namespace and class name
         
        \param      name_space
                the namespace of the desired C# class (nested namespaces separated by '.')
        \param      name
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
        \brief      Helper function to get the pointer to the C# Transform interface class

        \return     the pointer to the C# Transform MonoClass
        *//**********************************************************************************/
        MonoClass* GetTransformMonoClass();

        /*********************************************************************************//*!
        \brief      Helper function to create a new instance of a given class in C# side
         
        \param      klass
                the pointer to the MonoClass of the new object

        \return     the pointer to the newly created MonoObject (C# object)
        *//**********************************************************************************/
        MonoObject* MonoObjectNew(MonoClass* klass);

        /*********************************************************************************//*!
        \brief      Helper function to create a new string in C# side

        \param      text
                the desired text of the C# string

        \return     the pointer to the newly created MonoString (C# string)
        *//**********************************************************************************/
        MonoString* MonoStringNew(const char* text);

        /*********************************************************************************//*!
        \brief      Helper function to get the ScriptSystem's RegisteredComponent by C# Type

        \param      type
                the C# type of the desired registered component

        \return     the RegisteredComponent from ScriptSystem of the desired type
        *//**********************************************************************************/
        ScriptSystem::RegisteredComponent const& GetRegisteredComponent(MonoType* type);

        /*********************************************************************************//*!
        \brief      Helper function to get the index of the ScriptSystem's RegisteredComponent by C# Type

        \param      type
                the C# type of the desired registered component

        \return     the index of the RegisteredComponent from ScriptSystem of the desired type
        *//**********************************************************************************/
        size_t GetRegisteredComponentID(MonoType* type);

        /*********************************************************************************//*!
        \brief      Helper function to check if a given C# class field is marked public

        \param      field
                the pointer to the MonoClassField to check

        \return     true if the given field is public, else false
        *//**********************************************************************************/
        bool IsClassFieldPublic(MonoClassField* field);

        /*********************************************************************************//*!
        \brief      Helper function to check if a given C# class inherits from another C# class

        \param      klass
                the pointer to the MonoClass to check
        \param      desiredBase
                the pointer to the desired base MonoClass that klass may inherit from

        \return     true if the given class inherits from the given base class, else false
        *//**********************************************************************************/
        bool CheckBaseClass(MonoClass* klass, MonoClass* desiredBase);

        /*********************************************************************************//*!
        \brief      Helper function to check if a given C# type is a generic C# list

        \param      type
                the pointer to the MonoType to check

        \return     true if the given class is a generic C# list, else false
        *//**********************************************************************************/
        bool IsMonoTypeGenericList(MonoType* type);

        /*********************************************************************************//*!
        \brief      Helper function to get a function from a given C# object's class by name, if any

        \param      obj
                the pointer to the MonoObject to get the function from
        \param      functionName
                the name of the desired function
        \param      paramCount
                the number of parameters the desired function has

        \return     the pointer to the C# function found, or null if none were found
        *//**********************************************************************************/
        MonoMethod* FindFunction(MonoObject* obj, const char* functionName, int paramCount = 0);

        /*********************************************************************************//*!
        \brief      Helper function to get a list of C# classes used by a generic C# type

        \param      type
                the pointer to the MonoType to get the list of C# classes from

        \return     a vector of pointers to the MonoClasses used by the generic C# type
        *//**********************************************************************************/
        std::vector<MonoClass*> const GetMonoTypeGenericClasses(MonoType* type);
    }
}
