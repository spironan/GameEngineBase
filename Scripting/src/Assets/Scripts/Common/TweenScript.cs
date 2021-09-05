using System;
using System.Collections.Generic;
using System.Text;
using Ouroboros;

public enum TweenType
{
    Position,
    Rotation,
    Scale,
    Color,
    Alpha,
    Fill
}

public class TweenValue
{
    public string tweenName;
    public TweenType type;
    public float from;
    public float to;
    public float duration;
    public float startDelay;
    public bool isLoop;
}

public class TweenScript : MonoBehaviour
{
    public string enableTweenName = "ENABLE";
    
    // public Dictionary<string, float> test = new Dictionary<string, float>();
    //public TweenValue[] tweenArray = new TweenValue[5];
    //public TweenValue testValue;

    //private List<TweenValue> temp;

    private void Awake()
    {
        //int zero = 0;
        //int i = 5 / zero;
        //temp.Add(new TweenValue());
        //TweenValue value = null;
        //value.duration = 0;
        //tweenList.Add(new TweenValue());
    }
}