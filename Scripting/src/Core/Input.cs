using System.Numerics;
using System.Runtime.InteropServices;

namespace Ouroboros
{
    public class Input
    {
        [DllImport("__Internal")]
        private static extern bool AnyKey();
        
        public static bool anyKey { get { return AnyKey(); } }

        [DllImport("__Internal")]
        private static extern bool AnyKeyDown();

        public static bool anyKeyDown { get { return AnyKeyDown(); } }

        [DllImport("__Internal")]
        private static extern bool AnyKeyUp();

        public static bool anyKeyUp { get { return AnyKeyUp(); } }

        [DllImport("__Internal")]
        public static extern bool GetKey(KeyCode key);

        [DllImport("__Internal")]
        public static extern bool GetKeyDown(KeyCode key);

        [DllImport("__Internal")]
        public static extern bool GetKeyUp(KeyCode key);

        [DllImport("__Internal")]
        private static extern void GetMousePosition(out int x, out int y);

        public static Vector2 mousePosition
        {
            get
            {
                int x, y;
                GetMousePosition(out x, out y);
                return new Vector2(x, y);
            }
        }

        [DllImport("__Internal")]
        private static extern void GetMouseDelta(out int x, out int y);

        public static Vector2 mouseDelta
        {
            get
            {
                int x, y;
                GetMouseDelta(out x, out y);
                return new Vector2(x, y);
            }
        }

        [DllImport("__Internal")]
        private static extern bool AnyMouseButton();

        public static bool anyMouseButton { get { return AnyMouseButton(); } }

        [DllImport("__Internal")]
        private static extern bool AnyMouseButtonDown();

        public static bool anyMouseButtonDown { get { return AnyMouseButtonDown(); } }

        [DllImport("__Internal")]
        private static extern bool AnyMouseButtonUp();

        public static bool anyMouseButtonUp { get { return AnyMouseButtonUp(); } }

        [DllImport("__Internal")]
        public static extern bool GetMouseButton(MouseCode button);

        [DllImport("__Internal")]
        public static extern bool GetMouseButtonDown(MouseCode button);

        [DllImport("__Internal")]
        public static extern bool GetMouseButtonUp(MouseCode button);
    }
}