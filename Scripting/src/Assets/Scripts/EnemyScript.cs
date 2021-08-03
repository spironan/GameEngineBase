using System;
using Ouroboros;

public class EnemyScript : MonoBehaviour
{
    private int attackDamage;

    private void Awake()
    {
        Random rnd = new Random();
        attackDamage = rnd.Next(1, 11);
    }

    public int GetAttackDamage()
    {
        return attackDamage;
    }
}