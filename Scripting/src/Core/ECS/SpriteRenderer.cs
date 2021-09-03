using System;
using System.Numerics;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Ouroboros
{
    public class SpriteRenderer : Component
    {
        [DllImport("__Internal")] private static extern void Sprite2D_GetColor(int instanceID, out float r, out float g, out float b, out float a);
        [DllImport("__Internal")] private static extern void Sprite2D_SetColor(int instanceID, float r, float g, float b, float alpha = 1.0f);

        public Vector4 color
        {
            get
            {
                float r, g, b, a;
                Sprite2D_GetColor(gameObject.GetInstanceID(), out r, out g, out b, out a);
                return new Vector4(r, g, b, a);
            }
            set { Sprite2D_SetColor(gameObject.GetInstanceID(), value.X, value.Y, value.Z, value.W); }
        }
    }
}
