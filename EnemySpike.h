#pragma once

#include "Enemy.h"

class EnemySpike : public Enemy
{
    public:
    //Enemy enemy;
    int timer;
    float direction;
    float speed;
    EnemySpike();
    EnemySpike(Enemy enem, int xPos, int yPos, int num);
    void Die(std::vector<EnemySpike> &enemies);
    void Move();
};

EnemySpike::EnemySpike()
{

}

EnemySpike::EnemySpike(Enemy enem, int xPos, int yPos, int num) : Enemy(enem)
{
    timer = attackTimer;
    x = xPos;
    y = yPos;
    rect = { (float)x + 8, (float)y + 8, 48, 48 };
    id = num;
    iFrames = 0;

    int rand = GetRandomValue(0, 3);

    switch (rand)
    {
        case 0:
            direction = 45;
            break;
        case 1:
            direction = 135;
            break;
        case 2:
            direction = 225;
            break;
        case 3:
            direction = 315;
            break;
    }
    speed = 7;
}

void EnemySpike::Die(std::vector<EnemySpike> &enemies)
{
    if (GetRandomValue(1,2) == 2)
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

void EnemySpike::Move()
{
    x += cos((direction * PI) / 180) * speed;
    y += sin((direction * PI) / 180) * speed;
    rect = { (float)x + 8, (float)y + 8, 48, 48 };
}
