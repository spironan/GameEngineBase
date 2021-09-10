using System;
using System.Numerics;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Ouroboros
{
    public class Transform : Component
    {
        protected Transform() { }

        [DllImport("__Internal")] private static extern void Transform_GetLocalPosition(int instanceID, out float x, out float y, out float z);
        [DllImport("__Internal")] private static extern void Transform_SetLocalPosition(int instanceID, float x, float y, float z);

        public Vector3 localPosition
        {
            get
            {
                float x, y, z;
                Transform_GetLocalPosition(gameObject.GetInstanceID(), out x, out y, out z);
                return new Vector3(x, y, z);
            }
            set { Transform_SetLocalPosition(gameObject.GetInstanceID(), value.X, value.Y, value.Z); }
        }

        //[DllImport("__Internal")] private static extern void Transform_GetGlobalPosition(int instanceID, out float x, out float y, out float z);
        //[DllImport("__Internal")] private static extern void Transform_SetGlobalPosition(int instanceID, float x, float y, float z);

        //public Vector3 position
        //{
        //    get
        //    {
        //        float x, y, z;
        //        Transform_GetGlobalPosition(gameObject.GetInstanceID(), out x, out y, out z);
        //        return new Vector3(x, y, z);
        //    }
        //    set { Transform_SetGlobalPosition(gameObject.GetInstanceID(), value.X, value.Y, value.Z); }
        //}

        [DllImport("__Internal")] private static extern float Transform_GetLocalAngle(int instanceID);
        [DllImport("__Internal")] private static extern void Transform_SetLocalAngle(int instanceID, float angle);

        public float localAngle
        {
            get { return Transform_GetLocalAngle(gameObject.GetInstanceID()); }
            set { Transform_SetLocalAngle(gameObject.GetInstanceID(), value); }
        }

        [DllImport("__Internal")] private static extern void Transform_GetLocalScale(int instanceID, out float x, out float y, out float z);
        [DllImport("__Internal")] private static extern void Transform_SetLocalScale(int instanceID, float x, float y, float z);

        public Vector3 localScale
        {
            get
            {
                float x, y, z;
                Transform_GetLocalScale(gameObject.GetInstanceID(), out x, out y, out z);
                return new Vector3(x, y, z);
            }
            set { Transform_SetLocalScale(gameObject.GetInstanceID(), value.X, value.Y, value.Z); }
        }

        [DllImport("__Internal")] private static extern int Transform_GetChildCount(int instanceID);

        public int childCount
        {
            get { return Transform_GetChildCount(gameObject.GetInstanceID()); }
        }

        [DllImport("__Internal")] private static extern IntPtr Transform_GetChild(int instanceID, int childIndex);

        public Transform GetChild(int index)
        {
            IntPtr child = Transform_GetChild(gameObject.GetInstanceID(), index);
            if (child == IntPtr.Zero)
                return null;
            return (Transform)GCHandle.FromIntPtr(child).Target;
        }

        [DllImport("__Internal")] private static extern void Transform_SetParent(int instanceID, int newParent, bool preserveTransforms);
        [DllImport("__Internal")] private static extern IntPtr Transform_GetParent(int instanceID);

        public Transform parent
        {
            get
            {
                IntPtr ptr = Transform_GetParent(gameObject.GetInstanceID());
                if (ptr == IntPtr.Zero)
                    return null;
                return (Transform)GCHandle.FromIntPtr(ptr).Target;
            }
            set { SetParent(value); }
        }

        public void SetParent(Transform parent, bool worldPositionStays = false)
        {
            int parentID = (parent != null) ? parent.gameObject.GetInstanceID() : -1;
            Transform_SetParent(gameObject.GetInstanceID(), parentID, worldPositionStays);
        }
    }
}
