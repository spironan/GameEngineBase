using Ouroboros;

public class PlayerBodyScript : MonoBehaviour
{
    public float rotSpeed = 180.0f;
    public float changeDuration = 2.0f;
    private float angleDir;
    private float elapsed;

    private void Awake()
    {
        angleDir = 1;
        elapsed = 0;
    }

    private void Update(float dt)
    {
        elapsed += dt;
        if(elapsed >= changeDuration)
        {
            elapsed -= changeDuration;
            angleDir = -angleDir;
        }
        transform.localAngle += angleDir * rotSpeed * dt;
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        PlayerScript player = other.GetComponent<PlayerScript>();
        if (player != null)
            player.Die();
    }
}
