using System;
using System.Collections.Generic;
using Ouroboros;

public class EnemyManager : MonoBehaviour
{
    private List<EnemyScript> enemyList;

    private void Awake()
    {
        enemyList = new List<EnemyScript>();
        for(int i = 0; i < 10; ++i)
        {
            InstantiateEnemy();
        }
    }

    private EnemyScript InstantiateEnemy()
    {
        EnemyScript instance = GameObject.Instantiate().AddComponent<EnemyScript>();
        enemyList.Add(instance);
        return instance;
    }
}