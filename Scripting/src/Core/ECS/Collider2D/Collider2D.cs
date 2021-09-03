using System.Runtime.InteropServices;

namespace Ouroboros
{
    public class Collider2D : Component
    {
        [DllImport("__Internal")] private static extern bool Collider2D_GetIsTriggered(int instanceID);
        [DllImport("__Internal")] private static extern void Collider2D_SetIsTriggered(int instanceID, bool value);

        public bool isTrigger
        {
            get { return Collider2D_GetIsTriggered(gameObject.GetInstanceID()); }
            set { Collider2D_SetIsTriggered(gameObject.GetInstanceID(), value); }
        }
    }
}