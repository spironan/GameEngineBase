using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Ouroboros
{
    public class Component : Object
    {
        private GameObject m_GameObject = null;
        private int m_InstanceID = -1;

        public GameObject gameObject
        {
            get
            {
                return m_GameObject;
            }
        }

        public int GetInstanceID()
        {
            return m_InstanceID;
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

        #region Script/Component

        public T AddComponent<T>() where T : Component
        {
            return gameObject.AddComponent<T>();
        }

        public T GetComponent<T>() where T : Component
        {
            return gameObject.GetComponent<T>();
        }

        public void RemoveComponent<T>() where T : Component
        {
            gameObject.RemoveComponent<T>();
        }

        #endregion Script/Component
    }
}
