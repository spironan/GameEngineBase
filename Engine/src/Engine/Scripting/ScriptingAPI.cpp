#include "pch.h"
#include "ScriptingAPI.h"
#include "Engine/Core/Input.h"

SCRIPT_API_LINK(Scripting)

namespace engine
{
    namespace scriptAPI
    {
        /*-----------------------------------------------------------------------------*/
        /* Entity Functions for C#                                                     */
        /*-----------------------------------------------------------------------------*/

        SCRIPT_API Entity CreateEntity()
        {
            GameObject& instance = SceneManager::GetActiveScene().CreateGameObject();
            auto& scripting = instance.GetComponent<Scripting>();
            scripting.SetUpPlay();
            //scripting.StartPlay();
            return scripting.GetEntity();
        }

        SCRIPT_API uint32_t InstantiateEntity(Entity src)
        {
            engine::GameObject instance = GameObject::Instantiate(GameObject(src));
            auto& scripting = instance.GetComponent<Scripting>();
            scripting.SetUpPlay();
            scripting.StartPlay();
            return scripting.GetGameObjectPtr();
        }

        SCRIPT_API void DestroyEntity(Entity id)
        {
            GameObject obj{ id };
            obj.Destroy();
        }

        /*-----------------------------------------------------------------------------*/
        /* Script Functions for C#                                                     */
        /*-----------------------------------------------------------------------------*/

        SCRIPT_API uint32_t GameObject_GetName(Entity id)
        {
            GameObject obj{ id };
            std::string const& name = obj.GetComponent<GameObjectComponent>().Name;
            MonoString* string = ScriptUtility::MonoStringNew(name.c_str());
            return mono_gchandle_new((MonoObject*)string, false);
        }

        SCRIPT_API void GameObject_SetName(Entity id, const char* newName)
        {
            GameObject obj{ id };
            obj.GetComponent<GameObjectComponent>().Name = newName;
        }

        SCRIPT_API bool GameObject_GetActive(Entity id)
        {
            GameObject obj{ id };
            return obj.GetComponent<GameObjectComponent>().Active;
        }

        SCRIPT_API void GameObject_SetActive(Entity id, bool value)
        {
            GameObject obj{ id };
            obj.GetComponent<GameObjectComponent>().Active = value;
        }



        SCRIPT_API uint32_t AddScript(Entity id, const char* name_space, const char* name)
        {
            GameObject obj{ id };
            return obj.GetComponent<Scripting>().AddScript(name_space, name);
        }

        SCRIPT_API uint32_t GetScript(Entity id, const char* name_space, const char* name)
        {
            GameObject obj{ id };
            return obj.GetComponent<Scripting>().GetScript(name_space, name);
        }

        SCRIPT_API void RemoveScript(Entity id, const char* name_space, const char* name)
        {
            GameObject obj{ id };
            obj.GetComponent<Scripting>().RemoveScript(name_space, name);
        }

        SCRIPT_API void DestroyScript(Entity entityID, int scriptID)
        {
            GameObject obj{ entityID };
            obj.GetComponent<Scripting>().RemoveScript(scriptID);
        }

        SCRIPT_API void SetScriptEnabled(Entity entityID, int scriptID, bool enabled)
        {
            GameObject obj{ entityID };
            if (enabled)
                obj.GetComponent<Scripting>().EnableScript(scriptID);
            else
                obj.GetComponent<Scripting>().DisableScript(scriptID);
        }

        SCRIPT_API bool CheckScriptEnabled(Entity entityID, int scriptID)
        {
            GameObject obj{ entityID };
            return obj.GetComponent<Scripting>().GetScript(scriptID)->enabled;
        }



        SCRIPT_API uint32_t AddComponentFromScript(Entity id, const char* name_space, const char* name)
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

        SCRIPT_API uint32_t GetComponentFromScript(Entity id, const char* name_space, const char* name)
        {
            GameObject obj{ id };
            return obj.GetComponent<engine::Scripting>().GetComponentInterface(name_space, name);
        }

        SCRIPT_API void RemoveComponentFromScript(Entity id, const char* name_space, const char* name)
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

        /*-----------------------------------------------------------------------------*/
        /* Input Functions for C#                                                      */
        /*-----------------------------------------------------------------------------*/

        SCRIPT_API bool IsAnyKeyPressed()
        {
            return Input::IsAnyKeyPressed();
        }

        SCRIPT_API bool IsAnyKeyHeld()
        {
            return Input::IsAnyKeyHeld();
        }

        SCRIPT_API bool IsAnyKeyReleased()
        {
            return Input::IsAnyKeyReleased();
        }

        SCRIPT_API bool IsKeyPressed(int key)
        {
            return Input::IsKeyPressed(static_cast<KeyCode>(key));
        }

        SCRIPT_API bool IsKeyHeld(int key)
        {
            return Input::IsKeyHeld(static_cast<KeyCode>(key));
        }

        SCRIPT_API bool IsKeyReleased(int key)
        {
            return Input::IsKeyReleased(static_cast<KeyCode>(key));
        }

        SCRIPT_API void GetMousePosition(int* x, int* y)
        {
            std::pair<int, int> mousePos = Input::GetMousePosition();
            *x = mousePos.first;
            *y = mousePos.second;
        }

        SCRIPT_API void GetMouseDelta(int* x, int* y)
        {
            std::pair<int, int> mouseDelta = Input::GetMouseDelta();
            *x = mouseDelta.first;
            *y = mouseDelta.second;
        }

        SCRIPT_API bool IsAnyMouseButtonPressed()
        {
            return Input::IsAnyMouseButtonPressed();
        }

        SCRIPT_API bool IsAnyMouseButtonHeld()
        {
            return Input::IsAnyMouseButtonHeld();
        }

        SCRIPT_API bool IsAnyMouseButtonReleased()
        {
            return Input::IsAnyMouseButtonReleased();
        }

        SCRIPT_API bool IsMouseButtonPressed(int button)
        {
            return Input::IsMouseButtonPressed(static_cast<MouseCode>(button));
        }

        SCRIPT_API bool IsMouseButtonHeld(int button)
        {
            return Input::IsMouseButtonHeld(static_cast<MouseCode>(button));
        }

        SCRIPT_API bool IsMouseButtonReleased(int button)
        {
            return Input::IsMouseButtonReleased(static_cast<MouseCode>(button));
        }
    }
}
