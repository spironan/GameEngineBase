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
        private int instanceID = -1;

        public void DebugPrint()
        {
            Console.WriteLine("GAMEOBJECT " + instanceID);
        }

        #region Create/Destroy

        [DllImport("__Internal")]
        private static extern IntPtr CreateEntity();
        
        public static GameObject Instantiate()
        {
            return (GameObject)GCHandle.FromIntPtr(CreateEntity()).Target;
        }

        [DllImport("__Internal")]
        private static extern void DestroyEntity(int id);
        
        public static void Destroy(GameObject obj)
        {
            DestroyEntity(obj.instanceID);
        }

        #endregion Create/Destroy

        #region Component

        [DllImport("__Internal")]
        private static extern IntPtr AddScript(int id, string _namespace, string _type);

        public T AddComponent<T>() where T : Component
        {
            Type type = typeof(T);
            string _namespace = "";
            if (type.Namespace != null)
                _namespace = type.Namespace;

            IntPtr ptr = AddScript(instanceID, _namespace, type.Name);
            if (ptr == IntPtr.Zero)
                return null;
            return GCHandle.FromIntPtr(ptr).Target as T;
        }

        [DllImport("__Internal")]
        private static extern IntPtr GetScript(int id, string _namespace, string _type);

        public T GetComponent<T>() where T : Component
        {
            Type type = typeof(T);
            string _namespace = "";
            if (type.Namespace != null)
                _namespace = type.Namespace;

            IntPtr ptr = GetScript(instanceID, _namespace, type.Name);
            if (ptr == IntPtr.Zero)
                return null;
            return GCHandle.FromIntPtr(ptr).Target as T;
        }

        [DllImport("__Internal")]
        private static extern void RemoveScript(int id, string _namespace, string _type);

        public void RemoveComponent<T>() where T : Component
        {
            Type type = typeof(T);
            string _namespace = "";
            if (type.Namespace != null)
                _namespace = type.Namespace;

            RemoveScript(instanceID, _namespace, type.Name);
        }

        #endregion Component
    }
}