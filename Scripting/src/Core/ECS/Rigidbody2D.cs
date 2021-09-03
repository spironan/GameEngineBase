using System.Runtime.InteropServices;

namespace Ouroboros
{
    public class Rigidbody2D : Component
    {
        [DllImport("__Internal")] private static extern float Rigidbody2D_GetGravityScale(int instanceID);
        [DllImport("__Internal")] private static extern void Rigidbody2D_SetGravityScale(int instanceID, float value);

        public float gravityScale
        {
            get { return Rigidbody2D_GetGravityScale(gameObject.GetInstanceID()); }
            set { Rigidbody2D_SetGravityScale(gameObject.GetInstanceID(), value); }
        }
    }
}