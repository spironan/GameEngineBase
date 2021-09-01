using System;
using System.Runtime.InteropServices;

namespace Ouroboros
{
    public class Object
    {
        [DllImport("__Internal")] private static extern IntPtr CreateEntity();

        public static GameObject Instantiate()
        {
            return (GameObject)GCHandle.FromIntPtr(CreateEntity()).Target;
        }

        [DllImport("__Internal")] private static extern void DestroyEntity(int id);
        [DllImport("__Internal")] private static extern void DestroyScript(int entityID, int scriptID);
        [DllImport("__Internal")] private static extern void RemoveComponentFromScript(int id, string name_space, string name);

        public static void Destroy(Object obj)
        {
            Type type = obj.GetType();
            if(type == typeof(GameObject) || type.IsSubclassOf(typeof(GameObject))) // GameObject
            {
                GameObject gameObject = obj as GameObject;
                DestroyEntity(gameObject.GetInstanceID());
            }
            else if (type == typeof(MonoBehaviour) || type.IsSubclassOf(typeof(MonoBehaviour))) // MonoBehaviour (C# Scripts)
            {
                MonoBehaviour script = obj as MonoBehaviour;
                DestroyScript(script.gameObject.GetInstanceID(), script.GetInstanceID());
            }
            else if(type == typeof(Component) || type.IsSubclassOf(typeof(Component))) // C++ Components
            {
                Component component = obj as Component;
                RemoveComponentFromScript(component.gameObject.GetInstanceID(), type.Namespace ?? "", type.Name);
            }
        }
    }
}
