#pragma once

#include "Collectible.h"

class Coin : public Collectible
{
    public:
    int id;
    Coin();
    Coin(Texture2D text, int xPos, int yPos, int num);
    Coin(Texture2D text);
    void CollectCoin(Player &player);
    void Destroy(std::vector<Coin> &items);
    void CreateCoin(int xPos, int yPos);
};

extern std::vector<Coin> coinsLoaded;

Coin::Coin()
{
    
}

Coin::Coin(Texture2D text, int xPos, int yPos, int num)
{
    texture = text;
    x = xPos;
    y = yPos;
    rect = { (float)x, (float)y, 64, 64 };
    id = num;
}

Coin::Coin(Texture2D text)
{
    texture = text;
}

void Coin::CreateCoin(int xPos, int yPos)
{
    coinsLoaded.push_back(Coin(texture, xPos, yPos, coinsLoaded.size()));
}

void Coin::CollectCoin(Player &player)
{
    player.coins += GetRandomValue(2, 5);
}

void Coin::Destroy(std::vector<Coin> &items)
{
    items.erase(items.begin() + id);

    for (int i = id; i < (int)items.size(); i++)
    {
        items[i].id -= 1;
    }
}