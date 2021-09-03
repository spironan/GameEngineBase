using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Ouroboros
{
    public class MonoBehaviour : Component
    {
        protected MonoBehaviour()
        {

        }

        [DllImport("__Internal")] private static extern void SetScriptEnabled(int entityID, int scriptID, bool enabled);
        [DllImport("__Internal")] private static extern bool CheckScriptEnabled(int entityID, int scriptID);

        public bool enabled
        {
            get { return CheckScriptEnabled(gameObject.GetInstanceID(), GetInstanceID()); ; }
            set { SetScriptEnabled(gameObject.GetInstanceID(), GetInstanceID(), value); }
        }
    }
}