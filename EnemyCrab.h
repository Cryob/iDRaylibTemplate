#pragma once

#include "Enemy.h"

class EnemyCrab : public Enemy
{
    public:
    //Enemy enemy;
    int timer;
    int dashTimer;
    float direction;
    float speed;
    EnemyCrab();
    EnemyCrab(Enemy enem, int xPos, int yPos, int num);
    void Die(std::vector<EnemyCrab> &enemies);
    void AttemptAttack();
    void StartDash();
    void Dashing();
};

EnemyCrab::EnemyCrab()
{

}

EnemyCrab::EnemyCrab(Enemy enem, int xPos, int yPos, int num) : Enemy(enem)
{
    timer = attackTimer;
    x = xPos;
    y = yPos;
    rect = { (float)x + 8, (float)y + 8, 48, 48 };
    id = num;
    iFrames = 0;

    dashTimer = 0;
    speed = 8;
}

void EnemyCrab::Die(std::vector<EnemyCrab> &enemies)
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

void EnemyCrab::AttemptAttack()
{
    if (dashTimer > 0)
    {
        Dashing();
        dashTimer--;
        return;
    }

    if (timer > 0)
    {
        timer--;
    }
    else
    {
        if (Vector2Distance(Vector2{(float)player.x, (float)player.y}, Vector2{(float)x, (float)y}) < 300)
        {
            StartDash();
            timer = attackTimer;
        }
    }
}

void EnemyCrab::StartDash()
{
    dashTimer = 35;
    direction = GetDegreeDirection(Vector2{(float)x, (float)y}, Vector2{(float)player.x, (float)player.y}, false);
}

void EnemyCrab::Dashing()
{
    x += cos((direction * PI) / 180) * speed;
    y += sin((direction * PI) / 180) * speed;
    rect = { (float)x + 8, (float)y + 8, 48, 48 };
}