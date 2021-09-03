using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Ouroboros
{
    public class Component : Object
    {
        private GameObject m_GameObject = null;
        private int m_ComponentID = -1;

        public GameObject gameObject
        {
            get { return m_GameObject; }
        }

        public Transform transform
        {
            get { return gameObject.transform; }
        }

        public string name
        {
            get { return gameObject.name; }
            set { gameObject.name = value; }
        }

        public int GetComponentID()
        {
            return m_ComponentID;
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
