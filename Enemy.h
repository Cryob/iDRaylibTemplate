#pragma once

#include "Entity.h"
#include "Coin.h"
#include "Heart.h"

class Enemy : public Entity
{
    public:
    int id;
    int attackTimer;
    int contactDamage;
    Enemy();
    Enemy(Texture2D text, int HP, int atkTimer, int contactDMG);
    void SpawnCoin();
    void SpawnHeart();
    void Die(std::vector<Enemy> &enemies);
};

extern Player player;

extern Coin coinSpawner;
extern Heart heartSpawner;

Enemy::Enemy()
{

}

Enemy::Enemy(Texture2D text, int HP, int atkTimer, int contactDMG)
{
    texture = text;
    health = HP;
    attackTimer = atkTimer;
    contactDamage = contactDMG;
}

void Enemy::SpawnCoin()
{
    coinSpawner.CreateCoin(x, y);
}

void Enemy::SpawnHeart()
{
    heartSpawner.CreateHeart(x, y);
}
