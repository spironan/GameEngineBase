using Ouroboros;

public class EnemyScript : MonoBehaviour
{
    private int attackDamage;

    private void Awake()
    {
        attackDamage = Random.Range(1, 11);
    }

    public int GetAttackDamage()
    {
        return attackDamage;
    }
}