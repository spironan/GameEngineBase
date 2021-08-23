/************************************************************************************//*!
\file           Scripting.cpp
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
#include "pch.h"
#include "Scripting.h"
#include "ScriptUtility.h"

#include "Engine/ECS/WorldManager.h"
#include "Engine/ECS/GameObjectComponent.h"

namespace engine
{
    /*-----------------------------------------------------------------------------*/
    /* Helper Functions                                                            */
    /*-----------------------------------------------------------------------------*/

    /*********************************************************************************//*!
    \brief      sets the value of the field of a C# object to the given ScriptFieldValue value
     
    \param      obj
            the pointer to the MonoObject to edit
    \param      field
            the pointer to the specific MonoClassField of the MonoObject to edit
    \param      value
            the ScriptFieldValue containg the new field value to set
    *//**********************************************************************************/
    static void OverrideMonoField(MonoObject* obj, MonoClassField* field, ScriptFieldValue const& value)
    {
        switch (value.GetValueType())
        {
        case ScriptValueType::BOOL:
        {
            bool fieldValue = value.GetValue<bool>();
            mono_field_set_value(obj, field, &fieldValue);
        }
        break;
        case ScriptValueType::INT:
        {
            int fieldValue = value.GetValue<int>();
            mono_field_set_value(obj, field, &fieldValue);
        }
        break;
        case ScriptValueType::FLOAT:
        {
            float fieldValue = value.GetValue<float>();
            mono_field_set_value(obj, field, &fieldValue);
        }
        break;
        case ScriptValueType::STRING:
        {
            MonoString* fieldValue = ScriptUtility::MonoStringNew(value.GetValue<std::string>().c_str());
            mono_field_set_value(obj, field, fieldValue);
        }
        break;
        case ScriptValueType::GAMEOBJECT:
        {
            GameObject gameObject{ value.GetValue<Entity>() };
            Scripting* scripting = gameObject.TryGetComponent<Scripting>();
            if (scripting != nullptr)
            {
                MonoObject* monoGameObject = mono_gchandle_get_target(scripting->GetGameObjectPtr());
                mono_field_set_value(obj, field, monoGameObject);
            }
            else
            {
                mono_field_set_value(obj, field, nullptr);
            }
        }
        break;
        case ScriptValueType::CLASS:
        {
            MonoClass* classField = mono_type_get_class(mono_field_get_type(field));
            MonoObject* fieldValue = ScriptUtility::MonoObjectNew(classField);
            std::vector<ScriptFieldInfo> fieldInfoList = value.GetValue<std::vector<ScriptFieldInfo>>();
            for (ScriptFieldInfo const& fieldInfo : fieldInfoList)
            {
                MonoClassField* fieldValueField = mono_class_get_field_from_name(classField, fieldInfo.name.c_str());
                OverrideMonoField(fieldValue, fieldValueField, fieldInfo.value);
            }
            mono_field_set_value(obj, field, fieldValue);
        }
        break;
        case ScriptValueType::LIST:
        {
            MonoObject* fieldValue;
            mono_field_get_value(obj, field, &fieldValue);

            std::vector<MonoClass*> genericsList = ScriptUtility::GetMonoTypeGenericClasses(mono_field_get_type(field));
            MonoClass* valueClass = genericsList[0];

            MonoClass* fieldClass = mono_object_get_class(fieldValue);
            MonoMethod* addMethod = mono_class_get_method_from_name(fieldClass, "Add", 1);

            std::vector<ScriptFieldValue> valueList = value.GetValue<std::vector<ScriptFieldValue>>();
            for (ScriptFieldValue val : valueList)
            {
                if (valueClass == mono_get_boolean_class())
                {
                    bool entry = val.GetValue<bool>();
                    void* args[1];
                    args[0] = &entry;
                    mono_runtime_invoke(addMethod, fieldValue, args, NULL);
                }
                else if (valueClass == mono_get_int32_class())
                {
                    int entry = val.GetValue<int>();
                    void* args[1];
                    args[0] = &entry;
                    mono_runtime_invoke(addMethod, fieldValue, args, NULL);
                }
                else if (valueClass == mono_get_single_class())
                {
                    float entry = val.GetValue<float>();
                    void* args[1];
                    args[0] = &entry;
                    mono_runtime_invoke(addMethod, fieldValue, args, NULL);
                }
                else if (valueClass == mono_get_string_class())
                {
                    MonoString* entry = ScriptUtility::MonoStringNew(val.GetValue<std::string>().c_str());
                    void* args[1];
                    args[0] = entry;
                    mono_runtime_invoke(addMethod, fieldValue, args, NULL);
                }
                else
                {
                    MonoObject* entry = ScriptUtility::MonoObjectNew(valueClass);
                    mono_runtime_object_init(entry);

                    std::vector<ScriptFieldInfo> fieldInfoList = val.GetValue<std::vector<ScriptFieldInfo>>();
                    for (ScriptFieldInfo const& fieldInfo : fieldInfoList)
                    {
                        MonoClassField* fieldValueField = mono_class_get_field_from_name(valueClass, fieldInfo.name.c_str());
                        OverrideMonoField(entry, fieldValueField, fieldInfo.value);
                    }

                    void* args[1];
                    args[0] = entry;
                    mono_runtime_invoke(addMethod, fieldValue, args, NULL);
                }
            }
        }
        break;
        }
    }

    /*********************************************************************************//*!
    \brief      outputs the names and values of all the public fields of a given C# object
                to the standard output stream for Debugging purposes

    \param      object
            the MonoObject to print values of
    \param      depth
            the number of indentations to add
    *//**********************************************************************************/
    static void DebugPrintObjectFields(MonoObject* object, unsigned int depth)
    {
        MonoClass* compClass = mono_object_get_class(object);
        MonoType* compType = mono_class_get_type(compClass);
        for (unsigned int i = 0; i < depth; ++i)
            std::cout << "\t";
        std::cout << mono_type_full_name(compType) << std::endl;

        void* iter = NULL;
        MonoClassField* compField = nullptr;
        while ((compField = mono_class_get_fields(compClass, &iter)) != nullptr)
        {
            if (!ScriptUtility::IsClassFieldPublic(compField))
                continue;

            MonoType* type = mono_field_get_type(compField);
            MonoClass* typeClass = mono_type_get_class(type);
            int typeEnum = mono_type_get_type(type);

            switch (typeEnum)
            {
            case MONO_TYPE_BOOLEAN: // bool
            {
                bool fieldValue;
                mono_field_get_value(object, compField, &fieldValue);

                for (unsigned int i = 0; i < depth + 1; ++i)
                    std::cout << "\t";
                std::cout << "variable: " << mono_field_get_name(compField) << " (" << mono_type_get_name(type) << ", " << typeEnum << "): ";
                std::cout << fieldValue << std::endl;
            }
            break;
            case MONO_TYPE_I4: // int
            {
                int fieldValue;
                mono_field_get_value(object, compField, &fieldValue);

                for (unsigned int i = 0; i < depth + 1; ++i)
                    std::cout << "\t";
                std::cout << "variable: " << mono_field_get_name(compField) << " (" << mono_type_get_name(type) << ", " << typeEnum << "): ";
                std::cout << fieldValue << std::endl;
            }
            break;
            case MONO_TYPE_R4: // float
            {
                float fieldValue;
                mono_field_get_value(object, compField, &fieldValue);

                for (unsigned int i = 0; i < depth + 1; ++i)
                    std::cout << "\t";
                std::cout << "variable: " << mono_field_get_name(compField) << " (" << mono_type_get_name(type) << ", " << typeEnum << "): ";
                std::cout << fieldValue << std::endl;
            }
            break;
            case MONO_TYPE_STRING: // string
            {
                MonoString* fieldValue;
                mono_field_get_value(object, compField, &fieldValue);

                for (unsigned int i = 0; i < depth + 1; ++i)
                    std::cout << "\t";
                std::cout << "variable: " << mono_field_get_name(compField) << " (" << mono_type_get_name(type) << ", " << typeEnum << "): ";
                if (fieldValue != nullptr)
                    std::cout << std::string(mono_string_to_utf8(fieldValue)) << std::endl;
                else
                    std::cout << std::endl;
            }
            break;
            case MONO_TYPE_CLASS:
            {
                // for comparison
                MonoClass* GameObjectClass = ScriptUtility::GetGameObjectMonoClass();
                MonoClass* ComponentClass = ScriptUtility::GetBaseComponentMonoClass();

                MonoObject* fieldValue;
                mono_field_get_value(object, compField, &fieldValue);
                if (ScriptUtility::CheckBaseClass(typeClass, GameObjectClass)) // is a GameObject
                {
                    for (unsigned int i = 0; i < depth + 1; ++i)
                        std::cout << "\t";
                    std::cout << "variable: " << mono_field_get_name(compField) << " (" << mono_type_get_name(type) << ", " << typeEnum << "): ";

                    if (fieldValue != nullptr)
                    {
                        MonoClassField* idField = mono_class_get_field_from_name(typeClass, "m_instanceID");
                        int instanceID = -1;
                        mono_field_get_value(fieldValue, idField, &instanceID);
                        std::cout << "GAMEOBJECT " << instanceID << std::endl;
                    }
                    else
                    {
                        std::cout << "NULL" << std::endl;
                    }
                }
                else if (ScriptUtility::CheckBaseClass(typeClass, ComponentClass)) // is a GameObject Component
                {
                    for (unsigned int i = 0; i < depth + 1; ++i)
                        std::cout << "\t";
                    std::cout << "variable: " << mono_field_get_name(compField) << " (" << mono_type_get_name(type) << ", " << typeEnum << "): ";

                    if (fieldValue != nullptr)
                    {
                        MonoClassField* goField = mono_class_get_field_from_name(typeClass, "m_gameObject");
                        //std::cout << mono_field_get_name(goField);
                        MonoObject* go = nullptr;
                        mono_field_get_value(fieldValue, goField, &go);
                        //std::cout << mono_class_get_name(mono_object_get_class(go));
                        MonoClassField* idField = mono_class_get_field_from_name(mono_object_get_class(go), "m_instanceID");
                        int instanceID = -1;
                        mono_field_get_value(go, idField, &instanceID);
                        std::cout << "GAMEOBJECT " << instanceID << std::endl;
                    }
                    else
                    {
                        std::cout << "NULL" << std::endl;
                    }
                }
                else // is a container for info
                {
                    for (unsigned int i = 0; i < depth + 1; ++i)
                        std::cout << "\t";
                    std::cout << "variable: " << mono_field_get_name(compField) << " (" << mono_type_get_name(type) << ", " << typeEnum << "): ";
                    std::cout << std::endl;

                    if (fieldValue == nullptr)
                    {
                        fieldValue = ScriptUtility::MonoObjectNew(typeClass);
                        mono_runtime_object_init(fieldValue);
                    }
                    DebugPrintObjectFields(fieldValue, depth + 2);
                }
            }
            break;
            case MONO_TYPE_GENERICINST: // List
            {
                if (ScriptUtility::IsMonoTypeGenericList(type))
                {
                    MonoObject* fieldValue;
                    mono_field_get_value(object, compField, &fieldValue);
                    for (unsigned int i = 0; i < depth + 1; ++i)
                        std::cout << "\t";
                    std::cout << "variable: " << mono_field_get_name(compField) << " (" << mono_type_get_name(type) << ", " << typeEnum << "): ";
                    std::cout << std::endl;
                    if (fieldValue != nullptr)
                    {
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
                            MonoClass* itemClass = mono_object_get_class(item);
                            if (itemClass == mono_get_boolean_class())
                            {
                                for (unsigned int j = 0; j < depth + 2; ++j)
                                    std::cout << "\t";
                                std::cout << *(bool*)mono_object_unbox(item) << std::endl;
                            }
                            else if (itemClass == mono_get_int32_class())
                            {
                                for (unsigned int j = 0; j < depth + 2; ++j)
                                    std::cout << "\t";
                                std::cout << *(int*)mono_object_unbox(item) << std::endl;
                            }
                            else if (itemClass == mono_get_single_class())
                            {
                                for (unsigned int j = 0; j < depth + 2; ++j)
                                    std::cout << "\t";
                                std::cout << *(float*)mono_object_unbox(item) << std::endl;
                            }
                            else if (itemClass == mono_get_string_class())
                            {
                                for (unsigned int j = 0; j < depth + 2; ++j)
                                    std::cout << "\t";
                                std::cout << mono_string_to_utf8(mono_object_to_string(item, NULL)) << std::endl;
                            }
                            else
                            {
                                DebugPrintObjectFields(item, depth + 2);
                            }
                        }
                    }
                }
            }
            break;
            case MONO_TYPE_VALUETYPE: // Enum
            {
                int fieldValue;
                mono_field_get_value(object, compField, &fieldValue);

                MonoMethod* GetEnumNames = mono_class_get_method_from_name(mono_get_enum_class(), "GetNames", 1);
                void* args[1];
                args[0] = mono_type_get_object(ScriptUtility::g_SystemInfo.domain, type);
                MonoArray* enumNames = (MonoArray*)mono_runtime_invoke(GetEnumNames, NULL, args, NULL);

                for (unsigned int i = 0; i < depth + 1; ++i)
                    std::cout << "\t";
                std::cout << "variable: " << mono_field_get_name(compField) << " (" << mono_type_get_name(type) << ", " << typeEnum << "): ";
                std::cout << mono_string_to_utf8(mono_array_get(enumNames, MonoString*, fieldValue)) << std::endl;
                //std::cout << "All Possible Values: " << std::endl << "{" << std::endl;
                //unsigned int nameCount = (unsigned int)mono_array_length(enumNames);
                //for (unsigned int i = 0; i < nameCount; ++i)
                //{
                //    std::cout << mono_string_to_utf8(mono_array_get(enumNames, MonoString*, i)) << std::endl;
                //}
                //std::cout << "}" << std::endl;
            }
            break;
            //case MONO_TYPE_SZARRAY: // Array
            //{
            //    MonoArray* fieldValue;
            //    mono_field_get_value(object, compField, &fieldValue);
            //    std::cout << std::endl;
            //    if (fieldValue != nullptr)
            //    {
            //        uintptr_t arrayLength = mono_array_length(fieldValue);
            //        for (uintptr_t i = 0; i < arrayLength; ++i)
            //        {
            //            MonoObject* obj = mono_array_get(fieldValue, MonoObject*, i);
            //            if (obj == nullptr)
            //            {
            //                for (unsigned int j = 0; j < depth + 2; ++j)
            //                    std::cout << "\t";
            //                std::cout << "ELEMENT (" << i << "): NULL" << std::endl;
            //                continue;
            //            }
            //            DebugPrintObjectFields(obj, depth + 2);
            //        }
            //    }
            //}
            //break;
            }
        }
    }

    /*-----------------------------------------------------------------------------*/
    /* Constructors and Destructors                                                */
    /*-----------------------------------------------------------------------------*/

    Scripting::Scripting(Entity entityID, bool active) : Component{ entityID, active }, gameObjPtr{ 0 }
    {

    }

    Scripting::~Scripting()
    {
        StopPlay();
    }

    /*-----------------------------------------------------------------------------*/
    /* ECS Component Functions                                                     */
    /*-----------------------------------------------------------------------------*/
    uint32_t Scripting::AddComponentInterface(const char* name_space, const char* name)
    {
        // create Component interface
        MonoClass* compClass = ScriptUtility::GetMonoClass(name_space, name);
        MonoObject* component = ScriptUtility::MonoObjectNew(compClass);
        uint32_t componentPtr = mono_gchandle_new(component, false);
        mono_runtime_object_init(component);

        // set Component's gameObject
        MonoClassField* objField = mono_class_get_field_from_name(ScriptUtility::GetTransformMonoClass(), "m_gameObject");
        MonoObject* gameObject = mono_gchandle_get_target(gameObjPtr);
        mono_field_set_value(component, objField, gameObject);

        // store ptr in componentList
        size_t compID = ScriptUtility::GetRegisteredComponentID(mono_class_get_type(compClass));
        if (compID >= componentList.size())
            componentList.resize(compID + 1);
        componentList[compID] = componentPtr;

        return componentPtr;
    }

    uint32_t Scripting::GetComponentInterface(const char* name_space, const char* name)
    {
        MonoClass* compClass = ScriptUtility::GetMonoClass(name_space, name);
        size_t compID = ScriptUtility::GetRegisteredComponentID(mono_class_get_type(compClass));
        if (compID >= componentList.size())
            return 0;
        return componentList[compID];
    }

    void Scripting::RemoveComponentInterface(const char* name_space, const char* name)
    {
        MonoClass* compClass = ScriptUtility::GetMonoClass(name_space, name);
        size_t compID = ScriptUtility::GetRegisteredComponentID(mono_class_get_type(compClass));
        if (compID >= componentList.size() || componentList[compID] == 0)
            return;
        mono_gchandle_free(componentList[compID]);
        componentList[compID] = 0;
    }

    /*-----------------------------------------------------------------------------*/
    /* Script Functions                                                            */
    /*-----------------------------------------------------------------------------*/
    uint32_t Scripting::AddScript(const char* name_space, const char* name, bool callAwake)
    {
        if (!ScriptUtility::g_SystemInfo.IsSetUp() || gameObjPtr == 0)
            return 0;

        // create script instance
        MonoClass* klass = ScriptUtility::GetMonoClass(name_space, name);
        MonoObject* script = ScriptUtility::MonoObjectNew(klass);
        uint32_t scriptPtr = mono_gchandle_new(script, false);
        scriptList.push_back(scriptPtr);

        // call script constructor
        mono_runtime_object_init(script);

        // set gameObject field
        MonoClassField* gameObjectField = mono_class_get_field_from_name(klass, "m_gameObject");
        MonoObject* gameObject = mono_gchandle_get_target(gameObjPtr);
        mono_field_set_value(script, gameObjectField, gameObject);

        // call Awake, if wanted
        if (callAwake)
        {
            MonoMethod* awakeMethod = ScriptUtility::FindFunction(script, "Awake");
            if (awakeMethod != nullptr)
            {
                mono_runtime_invoke(awakeMethod, script, NULL, NULL);
            }
        }
        return scriptPtr;
    }

    uint32_t Scripting::GetScript(const char* name_space, const char* name)
    {
        if (!ScriptUtility::g_SystemInfo.IsSetUp() || gameObjPtr == 0)
            return 0;

        MonoClass* targetClass = ScriptUtility::GetMonoClass(name_space, name);
        if (targetClass == nullptr)
            return 0;

        for (unsigned int i = 0; i < scriptList.size(); ++i)
        {
            MonoObject* script = mono_gchandle_get_target(scriptList[i]);
            MonoClass* scriptClass = mono_object_get_class(script);
            if (scriptClass == targetClass)
                return scriptList[i];
        }
        return 0;
    }

    void Scripting::RemoveScript(const char* name_space, const char* name)
    {
        if (!ScriptUtility::g_SystemInfo.IsSetUp() || gameObjPtr == 0)
            return;

        MonoClass* targetClass = ScriptUtility::GetMonoClass(name_space, name);
        if (targetClass == nullptr)
            return;

        for (unsigned int i = 0; i < scriptList.size(); ++i)
        {
            MonoObject* script = mono_gchandle_get_target(scriptList[i]);
            MonoClass* scriptClass = mono_object_get_class(script);
            if (scriptClass != targetClass)
                continue;
            mono_gchandle_free(scriptList[i]);
            scriptList.erase(scriptList.begin() + i);
            break;
        }
    }

    /*-----------------------------------------------------------------------------*/
    /* Script Info Functions                                                       */
    /*-----------------------------------------------------------------------------*/
    ScriptInfo& Scripting::AddScriptInfo(ScriptClassInfo const& classInfo)
    {
        scriptInfoList.push_back(ScriptInfo{ classInfo });
        return scriptInfoList[scriptInfoList.size() - 1];
    }

    ScriptInfo* Scripting::GetScriptInfo(ScriptClassInfo const& classInfo)
    {
        for (unsigned int i = 0; i < scriptInfoList.size(); ++i)
        {
            if (scriptInfoList[i].classInfo == classInfo)
                return &scriptInfoList[i];
        }
        return nullptr;
    }

    std::vector<ScriptInfo>& Scripting::GetScriptInfoAll()
    {
        return scriptInfoList;
    }

    void Scripting::RemoveScriptInfo(ScriptClassInfo const& classInfo)
    {
        for (unsigned int i = 0; i < scriptInfoList.size(); ++i)
        {
            if (scriptInfoList[i].classInfo != classInfo)
                continue;
            scriptInfoList.erase(scriptInfoList.begin() + i);
            return;
        }
    }

    /*-----------------------------------------------------------------------------*/
    /* Mode Functions                                                              */
    /*-----------------------------------------------------------------------------*/
    void Scripting::SetUpPlay()
    {
        if (!ScriptUtility::g_SystemInfo.IsSetUp())
            return;

        // create C# gameObject
        MonoClass* _class = ScriptUtility::GetGameObjectMonoClass();
        MonoObject* gameObject = ScriptUtility::MonoObjectNew(_class);
        gameObjPtr = mono_gchandle_new(gameObject, false);
        mono_runtime_object_init(gameObject);

        // set GameObject instance id
        MonoClassField* idField = mono_class_get_field_from_name(_class, "m_instanceID");
        int id = GetEntity();
        mono_field_set_value(gameObject, idField, &id);

        // create Transform interface
        //MonoClass* transformClass = ScriptUtility::GetTransformMonoClass();
        //uint32_t transformPtr = AddComponentInterface(mono_class_get_namespace(transformClass), mono_class_get_name(transformClass));
        //MonoObject* transform = mono_gchandle_get_target(transformPtr);

        // set GameObject's transform
        //MonoClassField* transformField = mono_class_get_field_from_name(_class, "m_transform");
        //mono_field_set_value(gameObject, transformField, transform);

        // create all script instances
        for (ScriptInfo const& scriptInfo : scriptInfoList)
        {
            AddScript(scriptInfo.classInfo.name_space.c_str(), scriptInfo.classInfo.name.c_str(), false);
        }
    }

    void Scripting::StartPlay()
    {
        if (!ScriptUtility::g_SystemInfo.IsSetUp() || gameObjPtr == 0)
            return;

        // assign public variables and call Awake
        for (unsigned int i = 0; i < scriptInfoList.size(); ++i)
        {
            ScriptInfo& scriptInfo = scriptInfoList[i];
            MonoObject* scriptObj = mono_gchandle_get_target(scriptList[i]);
            MonoClass* scriptClass = ScriptUtility::GetMonoClass(scriptInfo.classInfo);
            for (ScriptFieldInfo fieldInfo : scriptInfo.fieldList)
            {
                MonoClassField* field = mono_class_get_field_from_name(scriptClass, fieldInfo.name.c_str());
                OverrideMonoField(scriptObj, field, fieldInfo.value);
            }
        }
        InvokeFunctionAll("Awake");
    }

    void Scripting::StopPlay()
    {
        if (gameObjPtr == 0)
            return;

        for (unsigned int i = 0; i < scriptList.size(); ++i)
        {
            mono_gchandle_free(scriptList[i]);
        }
        scriptList.clear();
        for (unsigned int i = 0; i < componentList.size(); ++i)
        {
            if (componentList[i] == 0)
                continue;
            mono_gchandle_free(componentList[i]);
        }
        componentList.clear();
        mono_gchandle_free(gameObjPtr);
        gameObjPtr = 0;
    }

    /*-----------------------------------------------------------------------------*/
    /* Function Invoking                                                           */
    /*-----------------------------------------------------------------------------*/
    void Scripting::InvokeFunctionAll(const char* functionName)
    {
        for (unsigned int i = 0; i < scriptList.size(); ++i)
        {
            MonoObject* script = mono_gchandle_get_target(scriptList[i]);
            MonoMethod* method = ScriptUtility::FindFunction(script, functionName);
            if (method == nullptr)
                continue;

            //std::cout << "GAMEOBJECT " << m_entity << " " << scriptInfoList[i].classInfo.name << ": " << functionName << std::endl;

            MonoObject* exception = nullptr;
            mono_runtime_invoke(method, script, NULL, &exception);
            if (exception)
            {
                MonoProperty* excMsgProperty = mono_class_get_property_from_name(mono_get_exception_class(), "Message");
                MonoString* excMsg = (MonoString*)mono_property_get_value(excMsgProperty, exception, NULL, NULL);

                LOG_CRITICAL(mono_string_to_utf8(excMsg));
                // mono_print_unhandled_exception(exception);
            }
            //__try
            //{
            //    mono_runtime_invoke(method, script, NULL, (MonoObject**)&exception);
            //}
            //__finally
            //{
            //    //std::cout << "SOMETHING" << std::endl;
            //    if (exception)
            //    {
            //        MonoProperty* excMsgProperty = mono_class_get_property_from_name(mono_get_exception_class(), "Message");
            //        MonoString* excMsg = (MonoString*)mono_property_get_value(excMsgProperty, exception, NULL, NULL);
            //        LOG_ERROR(mono_string_to_utf8(excMsg));
            //        mono_print_unhandled_exception(exception);
            //    }
            //}
        }
    }

    /*-----------------------------------------------------------------------------*/
    /* Getters                                                                     */
    /*-----------------------------------------------------------------------------*/
    uint32_t Scripting::GetGameObjectPtr()
    {
        return gameObjPtr;
    }

    /*-----------------------------------------------------------------------------*/
    /* Debugging Tools                                                             */
    /*-----------------------------------------------------------------------------*/
    void Scripting::DebugPrintInfo()
    {
        for (ScriptInfo const& scriptInfo : scriptInfoList)
        {
            scriptInfo.DebugPrint();
        }
    }

    void Scripting::DebugPrint()
    {
        std::cout << "GAMEOBJECT " << GetEntity() << std::endl;
        for (size_t i = 0; i < scriptList.size(); ++i)
        {
            MonoObject* script = mono_gchandle_get_target(scriptList[i]);
            DebugPrintObjectFields(script, 1);
        }
    }

    /*-----------------------------------------------------------------------------*/
    /* Script Functions for C#                                                     */
    /*-----------------------------------------------------------------------------*/
    uint32_t GameObject_GetName(int id)
    {
        std::string const& name = WorldManager::GetActiveWorld().GetComponent<GameObjectComponent>(id).Name;
        MonoString* string = ScriptUtility::MonoStringNew(name.c_str());
        return mono_gchandle_new((MonoObject*)string, false);
    }

    void GameObject_SetName(int id, const char* newName)
    {
        WorldManager::GetActiveWorld().GetComponent<GameObjectComponent>(id).Name = newName;
    }

    bool GameObject_GetActive(int id)
    {
        return WorldManager::GetActiveWorld().GetComponent<GameObjectComponent>(id).ActiveSelf;
    }

    void GameObject_SetActive(int id, bool value)
    {
        WorldManager::GetActiveWorld().GetComponent<GameObjectComponent>(id).ActiveSelf = value;
    }



    uint32_t AddScript(int id, const char* name_space, const char* name)
    {
        return WorldManager::GetActiveWorld().GetComponent<engine::Scripting>(id).AddScript(name_space, name);
    }

    uint32_t GetScript(int id, const char* name_space, const char* name)
    {
        return WorldManager::GetActiveWorld().GetComponent<engine::Scripting>(id).GetScript(name_space, name);
    }

    void RemoveScript(int id, const char* name_space, const char* name)
    {
        WorldManager::GetActiveWorld().GetComponent<engine::Scripting>(id).RemoveScript(name_space, name);
    }



    uint32_t AddComponentFromScript(int id, const char* name_space, const char* name)
    {
        uint32_t currPtr = WorldManager::GetActiveWorld().GetComponent<Scripting>(id).GetComponentInterface(name_space, name);
        if (currPtr != 0)
            return currPtr;

        MonoType* type = mono_class_get_type(ScriptUtility::GetMonoClass(name_space, name));
        ScriptSystem::RegisteredComponent const& component = ScriptUtility::GetRegisteredComponent(type);
        component.Add(id);
        return WorldManager::GetActiveWorld().GetComponent<Scripting>(id).AddComponentInterface(name_space, name);
    }

    uint32_t GetComponentFromScript(int id, const char* name_space, const char* name)
    {
        return WorldManager::GetActiveWorld().GetComponent<engine::Scripting>(id).GetComponentInterface(name_space, name);
    }

    void RemoveComponentFromScript(int id, const char* name_space, const char* name)
    {
        uint32_t currPtr = WorldManager::GetActiveWorld().GetComponent<Scripting>(id).GetComponentInterface(name_space, name);
        if (currPtr == 0)
            return;

        MonoType* type = mono_class_get_type(ScriptUtility::GetMonoClass(name_space, name));
        ScriptSystem::RegisteredComponent const& component = ScriptUtility::GetRegisteredComponent(type);
        component.Remove(id);
        WorldManager::GetActiveWorld().GetComponent<Scripting>(id).RemoveComponentInterface(name_space, name);
    }
}