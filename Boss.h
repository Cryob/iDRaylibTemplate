#pragma once

#include "Enemy.h"

class Boss : public Enemy
{
    public:
    int nextAttackTimer;
    int timer;
    int changeDirTimer;
    float direction;
    float speed;
    bool canAttack;
    bool active;
    bool dead;
    int actionTaken;
    int healthBarTimer;
    int introWaitTimer;
    Boss();
    Boss(Enemy enem, int xPos, int yPos, int num);
    void Die();
    void RunAI();
    void Move();
    void FillHealthBar();
    void DrawHealthBar();
};

extern std::vector<WallTile> loadedWallTiles;

Boss::Boss()
{

}

Boss::Boss(Enemy enem, int xPos, int yPos, int num) : Enemy(enem)
{
    x = xPos;
    y = yPos;
    id = num;
    rect = { (float)x, (float)y + 64, 128, 64 };
    active = false;
    canAttack = true;
    iFrames = 0;
    actionTaken = -1;
    speed = 8;
    nextAttackTimer = 60;
    dead = false;
    healthBarTimer = 0;
    introWaitTimer = 60;
}

void Boss::DrawHealthBar()
{
    DrawRectangle(20, 950, health * 2, 40, ORANGE);
}

void Boss::FillHealthBar()
{
    healthBarTimer += 2;
    DrawRectangle(20, 950, healthBarTimer * 2, 40, ORANGE);
}

void Boss::RunAI()
{
    if (health < 1)
    {
        dead = true;
    }

    if (nextAttackTimer > 1)
    {
        nextAttackTimer--;
        return;
    }

    if (canAttack)
    {
        actionTaken = GetRandomValue(0, 3);
        if (actionTaken == 0)
        {
            timer = GetRandomValue(120, 240);
            changeDirTimer = 20;
            direction = 0;
        }
        else if (actionTaken == 1)
        {
            timer = 120;
        }
        else if (actionTaken == 2)
        {
            timer = 180;
        }
        else if (actionTaken == 3)
        {
            actionTaken = 0;
            timer = GetRandomValue(120, 240);
            changeDirTimer = 20;
            direction = 0;
        }

        canAttack = false;
    }

    if (actionTaken == 0)
    {
        if (GetRandomValue(1, 20) == 20 && changeDirTimer < 1)
        {
            int randDirection = GetRandomValue(0, 7);
            direction = 45 * randDirection;
            changeDirTimer = 40;
        }
        else if (changeDirTimer > 0)
        {
            changeDirTimer--;
        }

        x += cos((direction * PI) / 180) * speed;
        rect = { (float)x, (float)y + 64, 128, 64 };

        for (WallTile tile : loadedWallTiles)
        {
            if (tile.CheckForCollision(rect))
            {
                if (x > 800)
                {
                    if (direction == 45)
                    {
                        direction += 90;
                        if (direction > 359)
                        {
                            direction = direction - 360;
                        }

                        x += cos((direction * PI) / 180) * speed;
                        break;
                    }
                    else if (direction == 315)
                    {
                        direction -= 90;
                        if (direction < 0)
                        {
                            direction = 360 + direction;
                        }

                        x += cos((direction * PI) / 180) * speed;
                        break;
                    }
                    else
                    {
                        direction = 180;
                        if (direction > 359)
                        {
                            direction = direction - 360;
                        }

                        x += cos((direction * PI) / 180) * speed;
                        break;
                    }
                }
                else
                {
                    if (direction == 135)
                    {
                        direction -= 90;
                        if (direction < 0)
                        {
                            direction = 360 + direction;
                        }

                        x += cos((direction * PI) / 180) * speed;
                        break;
                    }
                    else if (direction == 225)
                    {
                        direction += 90;
                        if (direction > 359)
                        {
                            direction = direction - 360;
                        }

                        x += cos((direction * PI) / 180) * speed;
                        break;
                    }
                    else
                    {
                        direction += 180;
                        if (direction > 359)
                        {
                            direction = direction - 360;
                        }

                        x += cos((direction * PI) / 180) * speed;
                        break;
                    }
                }
            }
        }

        y += sin((-direction * PI) / 180) * speed;
        rect = { (float)x, (float)y + 64, 128, 64 };

        for (WallTile tile : loadedWallTiles)
        {
            if (tile.CheckForCollision(rect))
            {
                if (y < 600)
                {
                    if (direction == 135)
                    {
                        direction += 90;
                        if (direction > 359)
                        {
                            direction = direction - 360;
                        }

                        y += sin((-direction * PI) / 180) * speed * 2;
                        break;
                    }
                    else if (direction == 45)
                    {
                        direction -= 90;
                        if (direction < 0)
                        {
                            direction = 360 + direction;
                        }

                        y += sin((-direction * PI) / 180) * speed * 2;
                        break;
                    }
                    else
                    {
                        direction += 180;
                        if (direction > 359)
                        {
                            direction = direction - 360;
                        }

                        y += sin((-direction * PI) / 180) * speed * 2;
                        break;
                    }
                }
                else
                {
                    if (direction == 225)
                    {
                        direction -= 90;
                        if (direction < 0)
                        {
                            direction = 360 + direction;
                        }

                        y += sin((-direction * PI) / 180) * speed * 2;
                        break;
                    }
                    else if (direction == 315)
                    {
                        direction += 90;
                        if (direction > 359)
                        {
                            direction = direction - 360;
                        }

                        y += sin((-direction * PI) / 180) * speed * 2;
                        break;
                    }
                    else
                    {
                        direction += 180;
                        if (direction > 359)
                        {
                            direction = direction - 360;
                        }

                        y += sin((-direction * PI) / 180) * speed * 2;
                        break;
                    }
                }
            }
        }

        timer--;

        if (timer < 0)
        {
            canAttack = true;
            nextAttackTimer = 40;
        }
    }
    else if (actionTaken == 1)
    {
        if (timer % 12 == 0)
        {
            float projDirection = GetDegreeDirection(Vector2{(float)x, (float)y}, Vector2{(float)player.x, (float)player.y}, true);
            projDirection += GetRandomValue(-15, 15);
            projSpawner.CreateProjectile(3, x + 64, y + 64, projDirection, false);
        }

        timer--;

        if (timer < 0)
        {
            canAttack = true;
            nextAttackTimer = 80;
        }
    }
    else if (actionTaken == 2)
    {
        if (timer % 40 == 0)
        {
            float projDirection = GetDegreeDirection(Vector2{(float)x, (float)y}, Vector2{(float)player.x, (float)player.y}, true);
            projSpawner.CreateProjectile(3, x + 64, y + 64, projDirection, false);
        }
        else if (timer % 40 == 20)
        {
            float projDirection = GetDegreeDirection(Vector2{(float)x, (float)y}, Vector2{(float)player.x, (float)player.y}, true);
            projDirection += 20;
            projSpawner.CreateProjectile(3, x + 64, y + 64, projDirection, false);

            projDirection = GetDegreeDirection(Vector2{(float)x, (float)y}, Vector2{(float)player.x, (float)player.y}, true);
            projDirection -= 20;
            projSpawner.CreateProjectile(3, x + 64, y + 64, projDirection, false);
        }

        timer--;

        if (timer < 0)
        {
            canAttack = true;
            nextAttackTimer = 80;
        }
    }
}