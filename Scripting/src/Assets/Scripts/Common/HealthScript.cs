using System;
using System.Collections.Generic;
using System.Text;
using Ouroboros;

class HealthScript : MonoBehaviour
{
    public int totalHealth = 100;
    public GameObject healthbar = null;

    private int currHealth = 0;

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