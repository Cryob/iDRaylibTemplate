#pragma once

#include "Enemy.h"

class EnemyPlant : public Enemy
{
    public:
    //Enemy enemy;
    int timer;
    EnemyPlant();
    EnemyPlant(Enemy enem, int xPos, int yPos, int num);
    void Die(std::vector<EnemyPlant> &enemies);
    void AttemptAttack();
    void Attack();
};

EnemyPlant::EnemyPlant()
{

}

EnemyPlant::EnemyPlant(Enemy enem, int xPos, int yPos, int num) : Enemy(enem)
{
    timer = attackTimer;
    x = xPos;
    y = yPos;
    rect = { (float)x, (float)y, 64, 64 };
    id = num;
    iFrames = 0;
}

void EnemyPlant::Die(std::vector<EnemyPlant> &enemies)
{
    if (GetRandomValue(1,3) == 3)
    {
        SpawnCoin();
    }
    else if (GetRandomValue(1,8) == 8)
    {
        SpawnHeart();
    }

    int oldID = id;

    enemies.erase(enemies.begin() + id);

    for (int i = oldID; i < (int)enemies.size(); i++)
    {
        enemies[i].id -= 1;
    }
}

void EnemyPlant::AttemptAttack()
{
    if (timer == 0)
    {
        Attack();
        timer = attackTimer;
    }
    else
    {
        timer--;
    }
}

void EnemyPlant::Attack()
{
    float direction = GetDegreeDirection(Vector2{(float)x, (float)y}, Vector2{(float)player.x, (float)player.y}, false);
    projSpawner.CreateProjectile(3, x, y, direction, false);
}