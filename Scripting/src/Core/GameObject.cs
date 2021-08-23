using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
// using System.Runtime.CompilerServices;

namespace Ouroboros
{
    // [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public class GameObject
    {
        private int m_instanceID = -1;
        private Transform m_transform = null;

        public GameObject() { }

        public int GetInstanceID()
        {
            return m_instanceID;
        }

        [DllImport("__Internal")] private static extern IntPtr GameObject_GetName(int id);
        [DllImport("__Internal")] private static extern void GameObject_SetName(int id, string newName);

        public string name
        {
            get
            { 
                GCHandle stringPtr = GCHandle.FromIntPtr(GameObject_GetName(m_instanceID));
                string name = (string)stringPtr.Target;
                stringPtr.Free();
                return name;
            }
            set { GameObject_SetName(m_instanceID, value); }
        }

        [DllImport("__Internal")] private static extern bool GameObject_GetActive(int id);
        [DllImport("__Internal")] private static extern void GameObject_SetActive(int id, bool value);

        public bool activeSelf
        {
            get { return GameObject_GetActive(m_instanceID); }
        }

        public void SetActive(bool value)
        {
            GameObject_SetActive(m_instanceID, value);
        }

        #region Create/Destroy

        [DllImport("__Internal")] private static extern IntPtr CreateEntity();
        
        public static GameObject Instantiate()
        {
            return (GameObject)GCHandle.FromIntPtr(CreateEntity()).Target;
        }

        [DllImport("__Internal")] private static extern void DestroyEntity(int id);
        
        public static void Destroy(GameObject obj)
        {
            DestroyEntity(obj.m_instanceID);
        }

        #endregion Create/Destroy

        #region Script/Component

        public Transform transform
        {
            get { return m_transform; }
        }

        [DllImport("__Internal")] private static extern IntPtr AddScript(int id, string name_space, string _type);
        [DllImport("__Internal")] private static extern IntPtr AddComponentFromScript(int id, string name_space, string name);

        public T AddComponent<T>() where T : Component
        {
            Type type = typeof(T);
            string name_space = "";
            if (type.Namespace != null)
                name_space = type.Namespace;

            IntPtr ptr = type.IsSubclassOf(typeof(MonoBehaviour))
                            ? AddScript(m_instanceID, name_space, type.Name)
                            : AddComponentFromScript(m_instanceID, name_space, type.Name);

            if (ptr == IntPtr.Zero)
                return null;
            return GCHandle.FromIntPtr(ptr).Target as T;
        }

        [DllImport("__Internal")] private static extern IntPtr GetScript(int id, string name_space, string _type);
        [DllImport("__Internal")] private static extern IntPtr GetComponentFromScript(int id, string name_space, string name);

        public T GetComponent<T>() where T : Component
        {
            Type type = typeof(T);
            string name_space = "";
            if (type.Namespace != null)
                name_space = type.Namespace;

            IntPtr ptr = type.IsSubclassOf(typeof(MonoBehaviour))
                            ? GetScript(m_instanceID, name_space, type.Name)
                            : GetComponentFromScript(m_instanceID, name_space, type.Name);

            if (ptr == IntPtr.Zero)
                return null;
            return GCHandle.FromIntPtr(ptr).Target as T;
        }

        [DllImport("__Internal")] private static extern void RemoveScript(int id, string name_space, string _type);
        [DllImport("__Internal")] private static extern void RemoveComponentFromScript(int id, string name_space, string name);

        public void RemoveComponent<T>() where T : Component
        {
            Type type = typeof(T);
            string name_space = "";
            if (type.Namespace != null)
                name_space = type.Namespace;

            if (type.IsSubclassOf(typeof(MonoBehaviour)))
            {
                RemoveScript(m_instanceID, name_space, type.Name);
            }
            else
            {
                RemoveComponentFromScript(m_instanceID, name_space, type.Name);
            }
        }

        #endregion Script/Component
    }
}