using System.Collections.Generic;
using System.Numerics;
using Ouroboros;

public class PlayerScript : MonoBehaviour
{
    public float moveSpeed = 50;
    private Vector3 moveDir;

    public float bodySize = 10.0f;
    public float bodySpacing = 5.0f;
    private float elapsed;

    private Transform bodyParent;
    private int score;

    private void Awake()
    {
        score = 0;
        moveDir = new Vector3(-1, 0, 0);
        bodyParent = new GameObject().transform;
        bodyParent.name = "TestParent";
    }

    public void Die()
    {
        ClearLength();
        score = 0;
        transform.localPosition = Vector3.Zero;
        moveDir = new Vector3(-1, 0, 0);
    }

    public void AddScore(int add)
    {
        score += add;
        Debug.Log("Score: " + score);
    }

    private void Update(float deltaTime)
    {
        if (Input.IsKeyPressed(KeyCode.UP))
        {
            moveDir = new Vector3(0, 1, 0);
        }
        if (Input.IsKeyPressed(KeyCode.DOWN))
        {
            moveDir = new Vector3(0, -1, 0);
        }
        if (Input.IsKeyPressed(KeyCode.LEFT))
        {
            moveDir = new Vector3(-1, 0, 0);
        }
        if (Input.IsKeyPressed(KeyCode.RIGHT))
        {
            moveDir = new Vector3(1, 0, 0);
        }

        elapsed += deltaTime;
        float moveDelay = (bodySize + bodySpacing) / moveSpeed;
        if (elapsed < moveDelay)
            return;
        elapsed -= moveDelay;
        transform.localPosition += moveDir * (bodySize + bodySpacing);
        if (bodyParent.childCount == 1)
            bodyParent.GetChild(0).localPosition = transform.localPosition + (-moveDir * (bodySize + bodySpacing));
        else if(bodyParent.childCount > 1)
        {
            Transform body = bodyParent.GetChild(bodyParent.childCount - 1);
            body.localPosition = transform.localPosition + (-moveDir * (bodySize + bodySpacing));
            body.parent = null;
            body.parent = bodyParent;
        }
    }

    public void AddLength()
    {
        GameObject instance = new GameObject("PlayerBody_" + (bodyParent.childCount + 1));
        instance.AddComponent<PlayerBodyScript>();
        instance.AddComponent<Rigidbody2D>().gravityScale = 0;
        instance.AddComponent<Collider2D>().isTrigger = true;
        instance.AddComponent<CircleCollider2D>();
        instance.AddComponent<SpriteRenderer>().color = new Vector4(0, 1.0f / (bodyParent.childCount + 2), 1, 1);
        instance.transform.localScale = new Vector3(10, 10, 1);
        Transform tail = (bodyParent.childCount > 0) ? bodyParent.GetChild(bodyParent.childCount - 1) : transform;
        instance.transform.localPosition = tail.localPosition + (-moveDir * (bodySize + bodySpacing));
        instance.transform.SetParent(bodyParent);
    }

    private void ClearLength()
    {
        while(bodyParent.childCount > 0)
        {
            //Destroy(bodyParent.GetChild(0).gameObject);
            GameObject body = bodyParent.GetChild(0).gameObject;
            body.transform.SetParent(null);
            Destroy(body);
        }
    }
}