using System;
using System.Collections.Generic;
using Ouroboros;

public class EnemyManager : MonoBehaviour
{
    //private List<EnemyScript> enemyList;
    //private GameObject prefab = null;

    //private void Awake()
    //{
    //    enemyList = new List<EnemyScript>();
    //    for (int i = 0; i < 10; ++i)
    //    {
    //        InstantiateEnemy();
    //    }
    //}

    //private EnemyScript InstantiateEnemy()
    //{
    //    if(prefab == null)
    //    {
    //        prefab = new GameObject("Enemy_" + (enemyList.Count + 1));
    //        enemyList.Add(prefab.AddComponent<EnemyScript>());
    //        return enemyList[enemyList.Count - 1];
    //    }
    //    EnemyScript instance = Instantiate(prefab).GetComponent<EnemyScript>();
    //    instance.name = "Enemy_" + (enemyList.Count + 1);
    //    enemyList.Add(instance);
    //    return instance;
    //}
}