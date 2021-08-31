/************************************************************************************//*!
\file           ScriptInfo.cpp
\project        <PROJECT_NAME>
\author         Solomon Tan Teng Shue, t.tengshuesolomon, 620010020
\par            email: t.tengshuesolomon\@digipen.edu
\date           August 4, 2021
\brief          Defines the functions declared in ScriptInfo.h to set and retrieve
                info of C# script instances that will be created during play mode

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "ScriptInfo.h"
#include "ScriptUtility.h"

#include "Utility/Hash.h"

namespace engine
{
    /*-----------------------------------------------------------------------------*/
    /* Helper Functions                                                            */
    /*-----------------------------------------------------------------------------*/

    // forward declaration
    ScriptFieldValue const GetMonoObjectValue(MonoObject* object);

    /*********************************************************************************//*!
    \brief      gets the value of a specific field in a C# object as a ScriptFieldValue
     
    \param      object
            the MonoObject to get the value from
    \param      field
            the specific MonoClassField of the MonoObject to get the value from
    
    \return     the value of the MonoField as a ScriptFieldValue
    *//**********************************************************************************/
    ScriptFieldValue const GetMonoFieldValue(MonoObject* object, MonoClassField* field)
    {
        MonoType* type = mono_field_get_type(field);
        int typeEnum = mono_type_get_type(type);
        switch (typeEnum)
        {
        case MONO_TYPE_BOOLEAN: // bool
            {
                bool fieldValue;
                mono_field_get_value(object, field, &fieldValue);
                return ScriptFieldValue(fieldValue);
            }
        case MONO_TYPE_I4: // int
        case MONO_TYPE_VALUETYPE: // Enum
            {
                int fieldValue;
                mono_field_get_value(object, field, &fieldValue);
                return ScriptFieldValue(fieldValue);
            }
        case MONO_TYPE_R4: // float
            {
                float fieldValue;
                mono_field_get_value(object, field, &fieldValue);
                return ScriptFieldValue(fieldValue);
            }
        case MONO_TYPE_STRING: // string
            {
                MonoString* fieldValue;
                mono_field_get_value(object, field, &fieldValue);
                if (fieldValue == nullptr)
                    return ScriptFieldValue(std::string());
                return ScriptFieldValue(std::string(mono_string_to_utf8(fieldValue)));
            }
        case MONO_TYPE_CLASS:
            {
                // for comparison
                MonoClass* typeClass = mono_type_get_class(type);
                MonoClass* GameObjectClass = ScriptUtility::GetGameObjectMonoClass();
                MonoClass* ComponentClass = ScriptUtility::GetBaseComponentMonoClass();

                if (ScriptUtility::CheckBaseClass(typeClass, GameObjectClass)) // is a GameObject
                {
                    return ScriptFieldValue((Entity)-1);
                }
                else if (ScriptUtility::CheckBaseClass(typeClass, ComponentClass)) // is a Component
                {
                    return ScriptFieldValue();
                }
                else // is a container for info
                {
                    MonoObject* fieldValue;
                    mono_field_get_value(object, field, &fieldValue);
                    if (fieldValue == nullptr)
                    {
                        fieldValue = ScriptUtility::MonoObjectNew(typeClass);
                        mono_runtime_object_init(fieldValue);
                    }

                    std::vector<ScriptFieldInfo> resultList;
                    void* iter = NULL;
                    MonoClassField* valueField = nullptr;
                    while ((valueField = mono_class_get_fields(typeClass, &iter)) != nullptr)
                    {
                        if (!ScriptUtility::IsClassFieldPublic(valueField))
                            continue;

                        std::string valueFieldName(mono_field_get_name(valueField));
                        ScriptFieldValue valueFieldValue = GetMonoFieldValue(fieldValue, valueField);

                        resultList.push_back({ valueFieldName, valueFieldValue });
                    }
                    return ScriptFieldValue(resultList);
                }
            }
        case MONO_TYPE_GENERICINST: // List
            {
                if (!ScriptUtility::IsMonoTypeGenericList(type))
                    return ScriptFieldValue();

                MonoObject* fieldValue;
                mono_field_get_value(object, field, &fieldValue);

                std::vector<ScriptFieldValue> resultList;

                if (fieldValue == nullptr)
                {
                    return ScriptFieldValue(resultList);
                }

                MonoClass* fieldClass = mono_object_get_class(fieldValue);
                MonoProperty* countProperty = mono_class_get_property_from_name(fieldClass, "Count");
                int count = *((int*)mono_object_unbox(mono_property_get_value(countProperty, fieldValue, NULL, NULL)));

                MonoProperty* accessProperty = mono_class_get_property_from_name(fieldClass, "Item");
                MonoMethod* accessMethod = mono_property_get_get_method(accessProperty);

                int i = 0;
                void* args[1];
                args[0] = &i;
                for (i = 0; i < count; ++i)
                {
                    MonoObject* item = mono_runtime_invoke(accessMethod, fieldValue, args, NULL);
                    resultList.push_back(GetMonoObjectValue(item));
                }
                return ScriptFieldValue(resultList);
            }
        }
        return ScriptFieldValue();
    }

    /*********************************************************************************//*!
    \brief      gets the value boxed in a given MonoObject pointer as a ScriptFieldValue
     
    \param      object
            the MonoObject containing the desired value
    
    \return     the value of the MonoObject as a ScriptFieldValue
    *//**********************************************************************************/
    ScriptFieldValue const GetMonoObjectValue(MonoObject* object)
    {
        MonoClass* objClass = mono_object_get_class(object);
        int typeEnum = mono_type_get_type(mono_class_get_type(objClass));

        switch (typeEnum)
        {
        case MONO_TYPE_BOOLEAN:
            return ScriptFieldValue(*(bool*)mono_object_unbox(object));
        case MONO_TYPE_I4: // int
        case MONO_TYPE_VALUETYPE: // Enum
            return ScriptFieldValue(*(int*)mono_object_unbox(object));
        case MONO_TYPE_R4: // float
            return ScriptFieldValue(*(float*)mono_object_unbox(object));
        case MONO_TYPE_STRING: // string
            return ScriptFieldValue(std::string(mono_string_to_utf8(mono_object_to_string(object, NULL))));
        case MONO_TYPE_CLASS:
            {
                // for comparison
                MonoClass* GameObjectClass = ScriptUtility::GetGameObjectMonoClass();
                MonoClass* ComponentClass = ScriptUtility::GetBaseComponentMonoClass();

                if (ScriptUtility::CheckBaseClass(objClass, GameObjectClass)) // is a GameObject
                {
                    return ScriptFieldValue((Entity)-1);
                }
                else if (ScriptUtility::CheckBaseClass(objClass, ComponentClass)) // is a Component
                {
                    return ScriptFieldValue();
                }
                else // is a container for info
                {
                    std::vector<ScriptFieldInfo> resultList;
                    void* iter = NULL;
                    MonoClassField* valueField = nullptr;
                    while ((valueField = mono_class_get_fields(objClass, &iter)) != nullptr)
                    {
                        if (!ScriptUtility::IsClassFieldPublic(valueField))
                            continue;

                        std::string valueFieldName(mono_field_get_name(valueField));
                        ScriptFieldValue valueFieldValue = GetMonoFieldValue(object, valueField);

                        resultList.push_back({ valueFieldName, valueFieldValue });
                    }
                    return ScriptFieldValue(resultList);
                }
            }
        case MONO_TYPE_GENERICINST: // List
            {
                if (!ScriptUtility::IsMonoTypeGenericList(mono_class_get_type(objClass)))
                    return ScriptFieldValue();

                std::vector<ScriptFieldValue> resultList;

                MonoProperty* countProperty = mono_class_get_property_from_name(objClass, "Count");
                int count = *((int*)mono_object_unbox(mono_property_get_value(countProperty, object, NULL, NULL)));

                MonoProperty* accessProperty = mono_class_get_property_from_name(objClass, "Item");
                MonoMethod* accessMethod = mono_property_get_get_method(accessProperty);

                int i = 0;
                void* args[1];
                args[0] = &i;
                for (i = 0; i < count; ++i)
                {
                    MonoObject* item = mono_runtime_invoke(accessMethod, object, args, NULL);
                    resultList.push_back(GetMonoObjectValue(item));
                }
                return ScriptFieldValue(resultList);
            }
        }
        return ScriptFieldValue();
    }

    /*-----------------------------------------------------------------------------*/
    /* ScriptInfo                                                                  */
    /*-----------------------------------------------------------------------------*/

    ScriptInfo::ScriptInfo(ScriptClassInfo const& _classInfo) : classInfo(_classInfo)
    {
        std::vector<ScriptFieldInfo> fieldList = classInfo.GetScriptFieldInfoAll();
        for (int i = 0; i < fieldList.size(); ++i)
        {
            fieldMap.insert({ utility::StringHash(fieldList[i].name), fieldList[i] });
        }
    }

    ScriptFieldInfo* ScriptInfo::FindFieldInfo(std::string const& fieldName)
    {
        utility::StringHash nameHash(fieldName);
        auto& search = fieldMap.find(nameHash);
        if(search == fieldMap.end())
            return nullptr;
        return &(search->second);
    }

    ScriptInfo& ScriptInfo::CopyFieldValues(ScriptInfo const& src)
    {
        for (auto& entry : fieldMap)
        {
            auto& search = src.fieldMap.find(entry.first);
            if (search == src.fieldMap.end())
                continue;
            if (entry.second.value.GetValueType() != search->second.value.GetValueType())
                continue;
            entry.second.value = search->second.value;
        }
        return *this;
    }

    void ScriptInfo::ResetFieldValues()
    {
        fieldMap.clear();
        std::vector<ScriptFieldInfo> fieldList = classInfo.GetScriptFieldInfoAll();
        for (int i = 0; i < fieldList.size(); ++i)
        {
            fieldMap.insert({ utility::StringHash(fieldList[i].name), fieldList[i] });
        }
    }

    void ScriptInfo::DebugPrint() const
    {
        if (classInfo.name_space.size() > 0)
        {
            std::cout << classInfo.name_space << "/";
        }
        std::cout << classInfo.name << std::endl;
        for (auto const& fieldInfo : fieldMap)
        {
            std::cout << fieldInfo.second << std::endl;
        }
    }

    /*-----------------------------------------------------------------------------*/
    /* ScriptClassInfo                                                             */
    /*-----------------------------------------------------------------------------*/

    std::vector<ScriptFieldInfo> const ScriptClassInfo::GetScriptFieldInfoAll()
    {
        MonoClass* _class = ScriptUtility::GetMonoClass(*this);
        // create sample instance with the default values
        MonoObject* sample = ScriptUtility::MonoObjectNew(_class);
        mono_runtime_object_init(sample);

        std::vector<ScriptFieldInfo> resultList;
        void* iter = NULL;
        MonoClassField* field = nullptr;
        while ((field = mono_class_get_fields(_class, &iter)) != nullptr)
        {
            if (!ScriptUtility::IsClassFieldPublic(field))
                continue;
            std::string fieldName(mono_field_get_name(field));
            ScriptFieldValue fieldValue = GetMonoFieldValue(sample, field);
            if (!fieldValue.IsNullType())
            {
                resultList.push_back({ fieldName, fieldValue });
            }
        }
        return resultList;
    }

    std::vector<ScriptClassInfo> const ScriptClassInfo::GetFieldGenericTypeParams(const char* fieldName)
    {
        MonoClass* _class = ScriptUtility::GetMonoClass(*this);
        MonoClassField* field = mono_class_get_field_from_name(_class, fieldName);
        MonoType* type = mono_field_get_type(field);
        std::vector<MonoClass*> genericsList = ScriptUtility::GetMonoTypeGenericClasses(type);
        std::vector<ScriptClassInfo> resultList;
        std::string name_space;
        std::string name;
        for (unsigned int i = 0; i < genericsList.size(); ++i)
        {
            name_space = mono_class_get_namespace(genericsList[i]);
            name = mono_class_get_name(genericsList[i]);
            resultList.push_back({ name_space, name });
        }
        return resultList;
    }

    bool ScriptClassInfo::operator==(ScriptClassInfo const& rhs)
    {
        return name_space == rhs.name_space && name == rhs.name;
    }

    bool ScriptClassInfo::operator!=(ScriptClassInfo const& rhs)
    {
        return name_space != rhs.name_space || name != rhs.name;
    }

    std::string const ScriptClassInfo::ToString() const
    {
        if (name_space.size() <= 0)
            return name;
        return name_space + "." + name;
    }

    /*-----------------------------------------------------------------------------*/
    /* ScriptFieldInfo                                                             */
    /*-----------------------------------------------------------------------------*/

    std::ostream& operator<<(std::ostream& os, ScriptFieldInfo const& fieldInfo)
    {
        os << fieldInfo.name << "(" << fieldInfo.value.GetValueType() << ")" << ": ";
        os << fieldInfo.value;
        return os;
    }

    /*-----------------------------------------------------------------------------*/
    /* ScriptFieldValue                                                            */
    /*-----------------------------------------------------------------------------*/

    std::ostream& operator<<(std::ostream& os, ScriptValueType value)
    {
        switch (value)
        {
        case ScriptValueType::BOOL: os << "Bool"; break;
        case ScriptValueType::INT: os << "Int"; break;
        case ScriptValueType::FLOAT: os << "Float"; break;
        case ScriptValueType::STRING: os << "String"; break;
        case ScriptValueType::GAMEOBJECT: os << "GameObject"; break;
        case ScriptValueType::CLASS: os << "Class"; break;
        case ScriptValueType::LIST: os << "List"; break;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, ScriptFieldValue const& value)
    {
        ScriptValueType type = value.GetValueType();
        switch (type)
        {
        case ScriptValueType::BOOL: os << value.GetValue<bool>(); break;
        case ScriptValueType::INT: os << value.GetValue<int>(); break;
        case ScriptValueType::FLOAT: os << value.GetValue<float>(); break;
        case ScriptValueType::STRING: os << value.GetValue<std::string>(); break;
        case ScriptValueType::GAMEOBJECT:
        {
            Entity entityValue = value.GetValue<Entity>();
            if (entityValue == (Entity)-1)
                os << "null";
            else
                os << "GAMEOBJECT " << entityValue;
        }
        break;
        case ScriptValueType::CLASS:
        {
            std::vector<ScriptFieldInfo> const& fieldValue = value.GetValue<std::vector<ScriptFieldInfo>>();
            for (unsigned int i = 0; i < fieldValue.size(); ++i)
            {
                os << " | " << fieldValue[i];
            }
            os << " | ";
        }
        break;
        case ScriptValueType::LIST:
        {
            std::vector<ScriptFieldValue> const& fieldValue = value.GetValue<std::vector<ScriptFieldValue>>();
            if (fieldValue.size() <= 0)
            {
                os << "EMPTY";
                break;
            }
            for (unsigned int i = 0; i < fieldValue.size(); ++i)
            {
                os << std::endl << fieldValue[i];
            }
        }
        break;
        default: os << "unknown"; break;
        }
        return os;
    }
}