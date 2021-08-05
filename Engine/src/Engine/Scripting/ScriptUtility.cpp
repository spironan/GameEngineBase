/************************************************************************************//*!
\file           ScriptUtility.cpp
\project        <PROJECT_NAME>
\author         Solomon Tan Teng Shue, t.tengshuesolomon, 620010020
\par            email: t.tengshuesolomon\@digipen.edu
\date           August 3, 2021
\brief          Defines any helper functions declared in ScriptUtility.h to make
                using the mono library easier

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/

#include "pch.h"
#include "ScriptUtility.h"

namespace engine
{
    namespace ScriptUtility
    {
        ScriptSystemInfo g_SystemInfo;

        /*-----------------------------------------------------------------------------*/
        /* Helper Functions                                                            */
        /*-----------------------------------------------------------------------------*/
        
        MonoClass* GetMonoClass(const char* name_space, const char* name)
        {
            return mono_class_from_name(g_SystemInfo.image, name_space, name);
        }

        MonoClass* GetMonoClass(ScriptClassInfo const& classInfo)
        {
            return mono_class_from_name(g_SystemInfo.image, classInfo.name_space.c_str(), classInfo.name.c_str());
        }

        MonoClass* GetGameObjectMonoClass()
        {
            return mono_class_from_name(g_SystemInfo.image, "Ouroboros", "GameObject");
        }

        MonoClass* GetBaseComponentMonoClass()
        {
            return mono_class_from_name(g_SystemInfo.image, "Ouroboros", "Component");
        }

        MonoClass* GetBaseScriptMonoClass()
        {
            return mono_class_from_name(g_SystemInfo.image, "Ouroboros", "MonoBehaviour");
        }
        
        MonoObject* MonoObjectNew(MonoClass* klass)
        {
            return mono_object_new(g_SystemInfo.domain, klass);
        }

        MonoString* MonoStringNew(const char* text)
        {
            return mono_string_new(g_SystemInfo.domain, text);
        }

        bool IsClassFieldPublic(MonoClassField* field)
        {
            MonoClass* testClass = mono_get_object_class();
            MonoMethod* testMethod = mono_class_get_method_from_name(testClass, "GetType", 0);
            return mono_method_can_access_field(testMethod, field);
        }

        bool CheckBaseClass(MonoClass* klass, MonoClass* desiredBase)
        {
            while (klass != nullptr)
            {
                if (klass == desiredBase)
                    return true;
                klass = mono_class_get_parent(klass);
            }
            return false;
        }

        MonoMethod* FindFunction(MonoObject* obj, const char* functionName)
        {
            MonoClass* _class = mono_object_get_class(obj);
            MonoMethod* _method = mono_class_get_method_from_name(_class, functionName, 0);
            MonoClass* _pClass = mono_class_get_parent(_class);

            while (_method == nullptr && _pClass != nullptr && _class != _pClass)
            {
                _class = _pClass;
                _pClass = mono_class_get_parent(_class);
                _method = mono_class_get_method_from_name(_class, functionName, 0);
            }
            return _method;
        }

        std::vector<MonoClass*> const GetMonoTypeGenericClasses(MonoType* type)
        {
            std::vector<MonoClass*> resultList;
            MonoObject* typeObj = (MonoObject*)mono_type_get_object(g_SystemInfo.domain, type);
            MonoClass* typeClass = mono_object_get_class(typeObj);
            MonoMethod* getMethod = mono_class_get_method_from_name(typeClass, "GetGenericArguments", 0);
            MonoArray* resultArray = (MonoArray*)mono_runtime_invoke(getMethod, typeObj, NULL, NULL);
            unsigned int arrayLength = mono_array_length(resultArray);
            for (unsigned int i = 0; i < arrayLength; ++i)
            {
                MonoReflectionType* genericType = mono_array_get(resultArray, MonoReflectionType*, i);
                MonoClass* genericClass = mono_type_get_class(mono_reflection_type_get_type(genericType));
                resultList.push_back(genericClass);
            }
            return resultList;
        }
    }
}
