using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
// using System.Runtime.CompilerServices;

namespace Ouroboros
{
    // [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public class GameObject : Object
    {
        private int m_InstanceID = -1;
        //private Transform m_Transform = null;

        public GameObject() { }

        public int GetInstanceID()
        {
            return m_InstanceID;
        }

        [DllImport("__Internal")] private static extern IntPtr GameObject_GetName(int id);
        [DllImport("__Internal")] private static extern void GameObject_SetName(int id, string newName);

        public string name
        {
            get
            { 
                GCHandle stringPtr = GCHandle.FromIntPtr(GameObject_GetName(m_InstanceID));
                string name = (string)stringPtr.Target;
                stringPtr.Free();
                return name;
            }
            set { GameObject_SetName(m_InstanceID, value); }
        }

        [DllImport("__Internal")] private static extern bool GameObject_GetActive(int id);
        [DllImport("__Internal")] private static extern void GameObject_SetActive(int id, bool value);

        public bool activeSelf
        {
            get { return GameObject_GetActive(m_InstanceID); }
        }

        public void SetActive(bool value)
        {
            GameObject_SetActive(m_InstanceID, value);
        }

        #region Script/Component

        //public Transform transform
        //{
        //    get { return m_Transform; }
        //}

        [DllImport("__Internal")] private static extern IntPtr AddScript(int id, string name_space, string _type);
        [DllImport("__Internal")] private static extern IntPtr AddComponentFromScript(int id, string name_space, string name);

        public T AddComponent<T>() where T : Component
        {
            Type type = typeof(T);
            string name_space = "";
            if (type.Namespace != null)
                name_space = type.Namespace;

            IntPtr ptr = type.IsSubclassOf(typeof(MonoBehaviour))
                            ? AddScript(m_InstanceID, name_space, type.Name)
                            : AddComponentFromScript(m_InstanceID, name_space, type.Name);

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
                            ? GetScript(m_InstanceID, name_space, type.Name)
                            : GetComponentFromScript(m_InstanceID, name_space, type.Name);

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
                RemoveScript(m_InstanceID, name_space, type.Name);
            }
            else
            {
                RemoveComponentFromScript(m_InstanceID, name_space, type.Name);
            }
        }

        #endregion Script/Component
    }
}