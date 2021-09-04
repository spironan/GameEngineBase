using Ouroboros;
using System.Numerics;

class FoodScript : MonoBehaviour
{
    public int minX = -6;
    public int maxX = 6;
    public int minY = -6;
    public int maxY = 6;

    private void Awake()
    {
        RandomizePosition();
    }

    private void RandomizePosition()
    {
        transform.localPosition = new Vector3(Random.Range(minX, maxX + 1) * 15.0f, Random.Range(minY, maxY + 1), 0.0f * 15.0f);
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        Debug.Log(name + " TRIGGER ENTER: " + other.name);
        PlayerScript player = other.GetComponent<PlayerScript>();
        if(player != null)
        {
            player.AddScore(10);
            player.AddLength();
            RandomizePosition();
        }
    }

    private void OnTriggerStay2D(Collider2D other)
    {
        //Debug.Log(name + " TRIGGER STAY: " + other.name);
    }

    private void OnTriggerExit2D(Collider2D other)
    {
        //Debug.Log(name + " TRIGGER EXIT: " + other.name);
    }
}