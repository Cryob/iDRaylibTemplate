#pragma once

#include "raylib.h"
#include "Player.h"
#include <vector>

class Collectible
{
    public:
    Texture2D texture;
    int x;
    int y;
    Rectangle rect;
    Collectible();
    Collectible(Texture2D text, int xPos, int yPos);
    bool CheckForCollision(Player player);
    void Draw();
    void DrawHitBox();
};

Collectible::Collectible()
{

}

Collectible::Collectible(Texture2D text, int xPos, int yPos)
{
    texture = text;
    x = xPos;
    y = yPos;
    rect = { (float)x, (float)y, 64, 64 };
}

bool Collectible::CheckForCollision(Player player)
{
    return CheckCollisionRecs(rect, player.rect);
}

void Collectible::Draw()
{
    DrawTexture(texture, x, y, WHITE);
}

void Collectible::DrawHitBox()
{
    DrawRectangleLinesEx(rect, 5, WHITE);
}
