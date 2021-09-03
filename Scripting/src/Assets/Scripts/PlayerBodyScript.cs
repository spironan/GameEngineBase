using Ouroboros;

public class PlayerBodyScript : MonoBehaviour
{
    private void OnTriggerEnter2D(Collider2D other)
    {
        PlayerScript player = other.GetComponent<PlayerScript>();
        if (player != null)
            player.Die();
    }
}
