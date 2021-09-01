using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Ouroboros
{
    public class Component
    {
        private GameObject m_GameObject = null;
        protected int m_InstanceID = -1;

        public GameObject gameObject
        {
            get
            {
                return m_GameObject;
            }
        }

        [DllImport("__Internal")] private static extern void SetScriptEnabled(int entityID, int scriptID, bool enabled);
        [DllImport("__Internal")] private static extern bool CheckScriptEnabled(int entityID, int scriptID);

        public bool enabled
        {
            get { return CheckScriptEnabled(gameObject.GetInstanceID(), m_InstanceID); }
            set { SetScriptEnabled(gameObject.GetInstanceID(), m_InstanceID, value); }
        }

        protected Component()
        {

        }
    }
}
