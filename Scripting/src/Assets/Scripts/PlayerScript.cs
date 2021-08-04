using System;
using Ouroboros;

public class PlayerScript : MonoBehaviour
{
    private HealthScript health;
    private TweenScript tween;

    private void Awake()
    {
        health = gameObject.GetComponent<HealthScript>();
        tween = gameObject.GetComponent<TweenScript>();
    }
}