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
