#pragma once

#include "Collectible.h"

class Heart : public Collectible
{
    public:
    int id;
    Heart();
    Heart(Texture2D text, int xPos, int yPos, int num);
    Heart(Texture2D text);
    void CollectHeart(Player &player);
    void Destroy(std::vector<Heart> &items);
    void CreateHeart(int xPos, int yPos);
};

extern std::vector<Heart> heartsLoaded;

Heart::Heart()
{
    
}

Heart::Heart(Texture2D text, int xPos, int yPos, int num)
{
    texture = text;
    x = xPos;
    y = yPos;
    rect = { (float)x, (float)y, 64, 64 };
    id = num;
}

Heart::Heart(Texture2D text)
{
    texture = text;
}

void Heart::CreateHeart(int xPos, int yPos)
{
    heartsLoaded.push_back(Heart(texture, xPos, yPos, heartsLoaded.size()));
}

void Heart::CollectHeart(Player &player)
{
    player.health += 2;
    if (player.health > 6)
    {
        player.health = 6;
    }
}

void Heart::Destroy(std::vector<Heart> &items)
{
    items.erase(items.begin() + id);

    for (int i = id; i < (int)items.size(); i++)
    {
        items[i].id -= 1;
    }
}