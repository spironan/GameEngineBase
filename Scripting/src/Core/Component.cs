using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Ouroboros
{
    public class Component
    {
        private GameObject _gameObject = null;

        public GameObject gameObject
        {
            get
            {
                return _gameObject;
            }
        }
    }
}
