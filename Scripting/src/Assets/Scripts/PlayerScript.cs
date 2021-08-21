using System.Numerics;
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

    private void Update()
    {
        if (Input.GetMouseButtonDown(MouseCode.ButtonLeft))
        {
            Debug.Log(Input.mousePosition);
        }
    }
}