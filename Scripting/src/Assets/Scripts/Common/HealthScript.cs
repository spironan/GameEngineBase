using System;
using System.Collections.Generic;
using System.Text;
using Ouroboros;

public class HealthScript : MonoBehaviour
{
    public int totalHealth = 100;
    public GameObject healthbar;

    private int currHealth = 0;

    private void Awake()
    {

    }

    public void TakeDamage(int dmg)
    {
        currHealth -= dmg;
        if (currHealth < 0)
            Die();
    }

    public void Die()
    {

    }
}