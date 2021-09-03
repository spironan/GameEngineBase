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
    }
}
