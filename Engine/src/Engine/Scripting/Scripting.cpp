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

#include "Utility/Hash.h"
#include "Engine/PhysicsCollision/ColliderCore.h"

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
            Entity entity = value.GetValue<Entity>();
            if (entity == -1)
                break;
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
                        MonoClassField* idField = mono_class_get_field_from_name(typeClass, "m_InstanceID");
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
                        MonoClassField* goField = mono_class_get_field_from_name(typeClass, "m_GameObject");
                        //std::cout << mono_field_get_name(goField);
                        MonoObject* go = nullptr;
                        mono_field_get_value(fieldValue, goField, &go);
                        //std::cout << mono_class_get_name(mono_object_get_class(go));
                        MonoClassField* idField = mono_class_get_field_from_name(mono_object_get_class(go), "m_InstanceID");
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

    Scripting::Scripting(Scripting const& ref) : Component{ ref.GetEntity(), ref.IsActive() }, gameObjPtr{ 0 }, scriptInfoMap{ ref.scriptInfoMap }
    {

    }

    Scripting::~Scripting()
    {
        StopPlay();
    }

    Component& Scripting::CopyComponent(Component const& comp)
    {
        auto& scripting = reinterpret_cast<Scripting const&>(comp);
        scriptInfoMap = scripting.scriptInfoMap;
        return *this;
    }

    /*-----------------------------------------------------------------------------*/
    /* ECS Component Functions                                                     */
    /*-----------------------------------------------------------------------------*/
    uint32_t Scripting::AddComponentInterface(const char* name_space, const char* name)
    {
        // if component interface is already created, return it
        MonoClass* compClass = ScriptUtility::GetMonoClass(name_space, name);
        ComponentType compID = ScriptUtility::GetRegisteredComponent(mono_class_get_type(compClass)).Index();
        if (compID < componentList.size() && componentList[compID] != 0)
            return componentList[compID];

        // create Component interface
        MonoObject* component = ScriptUtility::MonoObjectNew(compClass);
        uint32_t componentPtr = mono_gchandle_new(component, false);
        mono_runtime_object_init(component);

        // set Component's gameObject
        MonoClassField* objField = mono_class_get_field_from_name(compClass, "m_GameObject");
        MonoObject* gameObject = mono_gchandle_get_target(gameObjPtr);
        mono_field_set_value(component, objField, gameObject);

        // set Component's id
        MonoClassField* idField = mono_class_get_field_from_name(compClass, "m_ComponentID");
        mono_field_set_value(component, idField, &compID);

        // store ptr in componentList
        if (compID >= componentList.size())
            componentList.resize(compID + 1);
        componentList[compID] = componentPtr;

        return componentPtr;
    }

    uint32_t Scripting::GetComponentInterface(const char* name_space, const char* name)
    {
        MonoClass* compClass = ScriptUtility::GetMonoClass(name_space, name);
        ComponentType compID = ScriptUtility::GetRegisteredComponent(mono_class_get_type(compClass)).Index();
        if (compID >= componentList.size())
            return 0;
        return componentList[compID];
    }

    void Scripting::RemoveComponentInterface(const char* name_space, const char* name)
    {
        MonoClass* compClass = ScriptUtility::GetMonoClass(name_space, name);
        ComponentType compID = ScriptUtility::GetRegisteredComponent(mono_class_get_type(compClass)).Index();
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
        scriptList.push_back(ScriptInstance(scriptPtr));

        // call script constructor
        mono_runtime_object_init(script);

        // set gameObject field
        MonoClassField* gameObjectField = mono_class_get_field_from_name(klass, "m_GameObject");
        MonoObject* gameObject = mono_gchandle_get_target(gameObjPtr);
        mono_field_set_value(script, gameObjectField, gameObject);

        // set componentID field
        int id = scriptList.size() - 1;
        MonoClassField* idField = mono_class_get_field_from_name(klass, "m_ComponentID");
        mono_field_set_value(script, idField, &id);

        // call Awake, if wanted
        if (callAwake)
        {
            InvokeFunction(scriptPtr, "Awake");
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
            MonoObject* script = mono_gchandle_get_target(scriptList[i].handle);
            MonoClass* scriptClass = mono_object_get_class(script);
            if (scriptClass == targetClass)
                return scriptList[i].handle;
        }
        return 0;
    }

    Scripting::ScriptInstance const* Scripting::GetScript(int scriptID)
    {
        if (scriptID < 0 || scriptID >= scriptList.size())
            return nullptr;
        return &(scriptList[scriptID]);
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
            MonoObject* script = mono_gchandle_get_target(scriptList[i].handle);
            MonoClass* scriptClass = mono_object_get_class(script);
            if (scriptClass != targetClass)
                continue;
            mono_gchandle_free(scriptList[i].handle);
            scriptList.erase(scriptList.begin() + i);
            break;
        }
    }

    void Scripting::RemoveScript(int scriptID)
    {
        if (!ScriptUtility::g_SystemInfo.IsSetUp() || gameObjPtr == 0)
            return;
        if (scriptID < 0 || scriptID >= scriptList.size())
            return;
        mono_gchandle_free(scriptList[scriptID].handle);
        scriptList.erase(scriptList.begin() + scriptID);
    }

    void Scripting::EnableScript(uint32_t handle)
    {
        for (unsigned int i = 0; i < scriptList.size(); ++i)
        {
            if (scriptList[i].handle != handle)
                continue;
            EnableScript(i);
            return;
        }
    }

    void Scripting::EnableScript(int scriptID)
    {
        if (scriptID < 0 || scriptID >= scriptList.size() || scriptList[scriptID].enabled)
            return;
        scriptList[scriptID].enabled = true;
        InvokeFunction(scriptList[scriptID].handle, "OnEnable");
    }

    void Scripting::DisableScript(uint32_t handle)
    {
        for (unsigned int i = 0; i < scriptList.size(); ++i)
        {
            if (scriptList[i].handle != handle)
                continue;
            DisableScript(i);
            return;
        }
    }

    void Scripting::DisableScript(int scriptID)
    {
        if (scriptID < 0 || scriptID >= scriptList.size() || !scriptList[scriptID].enabled)
            return;
        InvokeFunction(scriptList[scriptID].handle, "OnDisable");
        scriptList[scriptID].enabled = false;
    }

    /*-----------------------------------------------------------------------------*/
    /* Script Info Functions                                                       */
    /*-----------------------------------------------------------------------------*/
    ScriptInfo& Scripting::AddScriptInfo(ScriptClassInfo const& classInfo)
    {
        auto& inserted = scriptInfoMap.insert({ utility::StringHash(classInfo.ToString()), ScriptInfo(classInfo) });
        return inserted.first->second;
    }

    ScriptInfo* Scripting::GetScriptInfo(ScriptClassInfo const& classInfo)
    {
        utility::StringHash key(classInfo.ToString());
        auto& search = scriptInfoMap.find(key);
        if(search == scriptInfoMap.end())
            return nullptr;
        return &(search->second);
    }

    std::map<unsigned int, ScriptInfo>& Scripting::GetScriptInfoAll()
    {
        return scriptInfoMap;
    }

    void Scripting::RemoveScriptInfo(ScriptClassInfo const& classInfo)
    {
        utility::StringHash key(classInfo.ToString());
        auto& search = scriptInfoMap.find(key);
        if (search == scriptInfoMap.end())
            return;
        scriptInfoMap.erase(search);
    }

    void Scripting::RefreshScriptInfoAll()
    {
        for (auto& entry : scriptInfoMap)
        {
            ScriptInfo newInfo(entry.second.classInfo);
            newInfo.CopyFieldValues(entry.second);
            entry.second = newInfo;
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
        // mono_runtime_object_init(gameObject);

        // set GameObject instance id
        MonoClassField* idField = mono_class_get_field_from_name(_class, "m_InstanceID");
        Entity id = GetEntity();
        mono_field_set_value(gameObject, idField, &id);

        // create Transform interface
        MonoClass* transformClass = ScriptUtility::GetTransformMonoClass();
        uint32_t transformPtr = AddComponentInterface(mono_class_get_namespace(transformClass), mono_class_get_name(transformClass));
        MonoObject* transform = mono_gchandle_get_target(transformPtr);

        // set GameObject's transform
        MonoClassField* transformField = mono_class_get_field_from_name(_class, "m_Transform");
        mono_field_set_value(gameObject, transformField, transform);

        // add all other Components
        for (auto const& component : ScriptUtility::g_SystemInfo.componentMap)
        {
            if (component.second.Has(GetEntity()))
            {
                MonoClass* _class = mono_type_get_class(component.first);
                const char* name_space = mono_class_get_namespace(_class);
                const char* name = mono_class_get_name(_class);
                AddComponentInterface(name_space, name);
            }
        }

        // create all script instances
        for (auto const& scriptInfo : scriptInfoMap)
        {
            AddScript(scriptInfo.second.classInfo.name_space.c_str(), scriptInfo.second.classInfo.name.c_str(), false);
        }
    }

    void Scripting::StartPlay()
    {
        if (!ScriptUtility::g_SystemInfo.IsSetUp() || gameObjPtr == 0)
            return;

        // assign public variables and call Awake
        int scriptIndex = 0;
        for (auto const& scriptInfo : scriptInfoMap)
        {
            MonoObject* scriptObj = mono_gchandle_get_target(scriptList[scriptIndex].handle);
            MonoClass* scriptClass = ScriptUtility::GetMonoClass(scriptInfo.second.classInfo);
            for (auto const& fieldInfo : scriptInfo.second.fieldMap)
            {
                MonoClassField* field = mono_class_get_field_from_name(scriptClass, fieldInfo.second.name.c_str());
                OverrideMonoField(scriptObj, field, fieldInfo.second.value);
            }
            ++scriptIndex;
        }
        InvokeFunctionAll("Awake");
    }

    void Scripting::StopPlay()
    {
        if (gameObjPtr == 0)
            return;

        for (unsigned int i = 0; i < scriptList.size(); ++i)
        {
            mono_gchandle_free(scriptList[i].handle);
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
    void Scripting::InvokeFunction(uint32_t pointer, const char* functionName, int paramCount, void** params)
    {
        MonoObject* script = mono_gchandle_get_target(pointer);
        MonoMethod* method = ScriptUtility::FindFunction(script, functionName, paramCount);
        if (method == nullptr)
            return;

        //std::cout << "GAMEOBJECT " << m_entity << " " << scriptInfoList[i].classInfo.name << ": " << functionName << std::endl;

        MonoObject* exception = nullptr;
        mono_runtime_invoke(method, script, params, &exception);
        if (exception)
        {
            MonoMethod* stringMethod = mono_class_get_method_from_name(mono_get_exception_class(), "ToString", 0);
            MonoString* excString = (MonoString*)mono_runtime_invoke(stringMethod, exception, NULL, NULL);
            LOG_CRITICAL(mono_string_to_utf8(excString));
        }
    }

    void Scripting::InvokeFunctionAll(const char* functionName, int paramCount, void** params)
    {
        for (unsigned int i = 0; i < scriptList.size(); ++i)
        {
            if (!scriptList[i].enabled)
                continue;
            InvokeFunction(scriptList[i].handle, functionName, paramCount, params);
        }
    }

    void Scripting::InvokeTriggerEnter2D(Scripting& other)
    {
        if (gameObjPtr == 0 || other.gameObjPtr == 0)
            return;
        GameObject obj{ other.GetEntity() };
        ComponentType compID = obj.GetComponentType<Collider2D>();
        if (compID >= other.componentList.size() || compID == 0)
            return;
        MonoObject* comp = mono_gchandle_get_target(other.componentList[compID]);
        void* params[1] = { comp };
        InvokeFunctionAll("OnTriggerEnter2D", 1, params);
    }

    void Scripting::InvokeTriggerStay2D(Scripting& other)
    {
        if (gameObjPtr == 0 || other.gameObjPtr == 0)
            return;
        GameObject obj{ other.GetEntity() };
        ComponentType compID = obj.GetComponentType<Collider2D>();
        if (compID >= other.componentList.size() || compID == 0)
            return;
        MonoObject* comp = mono_gchandle_get_target(other.componentList[compID]);
        void* params[1] = { comp };
        InvokeFunctionAll("OnTriggerStay2D", 1, params);
    }

    void Scripting::InvokeTriggerExit2D(Scripting& other)
    {
        if (gameObjPtr == 0 || other.gameObjPtr == 0)
            return;
        GameObject obj{ other.GetEntity() };
        ComponentType compID = obj.GetComponentType<Collider2D>();
        if (compID >= other.componentList.size() || compID == 0)
            return;
        MonoObject* comp = mono_gchandle_get_target(other.componentList[compID]);
        void* params[1] = { comp };
        InvokeFunctionAll("OnTriggerExit2D", 1, params);
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
        for (auto const& scriptInfo : scriptInfoMap)
        {
            scriptInfo.second.DebugPrint();
        }
    }

    void Scripting::DebugPrint()
    {
        std::cout << "GAMEOBJECT " << GetEntity() << std::endl;
        for (size_t i = 0; i < scriptList.size(); ++i)
        {
            MonoObject* script = mono_gchandle_get_target(scriptList[i].handle);
            DebugPrintObjectFields(script, 1);
        }
    }

    /*-----------------------------------------------------------------------------*/
    /* Script Functions for C#                                                     */
    /*-----------------------------------------------------------------------------*/
    uint32_t GameObject_GetName(Entity id)
    {
        GameObject obj{ id };
        std::string const& name = obj.Name();
        MonoString* string = ScriptUtility::MonoStringNew(name.c_str());
        return mono_gchandle_new((MonoObject*)string, false);
    }

    void GameObject_SetName(Entity id, const char* newName)
    {
        GameObject obj{ id };
        obj.Name() = newName;
    }

    bool GameObject_GetActive(Entity id)
    {
        GameObject obj{ id };
        return obj.Active();
    }

    void GameObject_SetActive(Entity id, bool value)
    {
        GameObject obj{ id };
        obj.Active() = value;
    }



    uint32_t AddScript(Entity id, const char* name_space, const char* name)
    {
        GameObject obj{ id };
        return obj.GetComponent<Scripting>().AddScript(name_space, name);
    }

    uint32_t GetScript(Entity id, const char* name_space, const char* name)
    {
        GameObject obj{ id };
        return obj.GetComponent<Scripting>().GetScript(name_space, name);
    }

    void RemoveScript(Entity id, const char* name_space, const char* name)
    {
        GameObject obj{ id };
        obj.GetComponent<Scripting>().RemoveScript(name_space, name);
    }

    void DestroyScript(Entity entityID, int scriptID)
    {
        GameObject obj{ entityID };
        obj.GetComponent<Scripting>().RemoveScript(scriptID);
    }

    void SetScriptEnabled(Entity entityID, int scriptID, bool enabled)
    {
        GameObject obj{ entityID };
        if(enabled)
            obj.GetComponent<Scripting>().EnableScript(scriptID);
        else
            obj.GetComponent<Scripting>().DisableScript(scriptID);
    }

    bool CheckScriptEnabled(Entity entityID, int scriptID)
    {
        GameObject obj{ entityID };
        return obj.GetComponent<Scripting>().GetScript(scriptID)->enabled;
    }



    uint32_t AddComponentFromScript(Entity id, const char* name_space, const char* name)
    {
        GameObject obj{ id };
        uint32_t currPtr = obj.GetComponent<Scripting>().GetComponentInterface(name_space, name);
        if (currPtr != 0)
            return currPtr;

        MonoType* type = mono_class_get_type(ScriptUtility::GetMonoClass(name_space, name));
        ScriptSystem::RegisteredComponent const& component = ScriptUtility::GetRegisteredComponent(type);
        component.Add(id);
        return obj.GetComponent<Scripting>().AddComponentInterface(name_space, name);
    }

    uint32_t GetComponentFromScript(Entity id, const char* name_space, const char* name)
    {
        GameObject obj{ id };
        return obj.GetComponent<engine::Scripting>().GetComponentInterface(name_space, name);
    }

    void RemoveComponentFromScript(Entity id, const char* name_space, const char* name)
    {
        GameObject obj{ id };
        uint32_t currPtr = obj.GetComponent<Scripting>().GetComponentInterface(name_space, name);
        if (currPtr == 0)
            return;

        MonoType* type = mono_class_get_type(ScriptUtility::GetMonoClass(name_space, name));
        ScriptSystem::RegisteredComponent const& component = ScriptUtility::GetRegisteredComponent(type);
        component.Remove(id);
        obj.GetComponent<Scripting>().RemoveComponentInterface(name_space, name);
    }
}