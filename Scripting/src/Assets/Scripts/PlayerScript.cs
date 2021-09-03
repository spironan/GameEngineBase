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

    private List<Transform> bodyList;
    private int score;

    private void Awake()
    {
        bodyList = new List<Transform>();
        score = 0;
        moveDir = new Vector3(-1, 0, 0);
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
        AddLength();
        Debug.Log("Score: " + score);
    }

    private void Update(float deltaTime)
    {
        if (Input.GetKeyDown(KeyCode.UP))
        {
            moveDir = new Vector3(0, 1, 0);
        }
        if (Input.GetKeyDown(KeyCode.DOWN))
        {
            moveDir = new Vector3(0, -1, 0);
        }
        if (Input.GetKeyDown(KeyCode.LEFT))
        {
            moveDir = new Vector3(-1, 0, 0);
        }
        if (Input.GetKeyDown(KeyCode.RIGHT))
        {
            moveDir = new Vector3(1, 0, 0);
        }

        elapsed += deltaTime;
        float moveDelay = (bodySize + bodySpacing) / moveSpeed;
        if (elapsed < moveDelay)
            return;
        elapsed -= moveDelay;
        transform.localPosition += moveDir * (bodySize + bodySpacing);
        if (bodyList.Count == 1)
            bodyList[0].localPosition = transform.localPosition + (-moveDir * (bodySize + bodySpacing));
        else if(bodyList.Count > 1)
        {
            Transform body = bodyList[bodyList.Count - 1];
            body.localPosition = transform.localPosition + (-moveDir * (bodySize + bodySpacing));
            bodyList.RemoveAt(bodyList.Count - 1);
            bodyList.Insert(0, body);
        }
    }

    private void AddLength()
    {
        GameObject instance = new GameObject("PlayerBody_" + (bodyList.Count + 1));
        instance.AddComponent<PlayerBodyScript>();
        instance.AddComponent<Rigidbody2D>().gravityScale = 0;
        instance.AddComponent<Collider2D>().isTrigger = true;
        instance.AddComponent<CircleCollider2D>();
        instance.AddComponent<SpriteRenderer>().color = new Vector4(0, 1.0f / (bodyList.Count + 2), 1, 1);
        instance.transform.localScale = new Vector3(10, 10, 1);
        Transform tail = (bodyList.Count > 0) ? bodyList[bodyList.Count - 1] : transform;
        instance.transform.localPosition = tail.localPosition + (-moveDir * (bodySize + bodySpacing));
        bodyList.Add(instance.transform);
    }

    private void ClearLength()
    {
        for (int i = 0; i < bodyList.Count; ++i)
        {
            Destroy(bodyList[i].gameObject);
        }
        bodyList.Clear();
    }
}