#pragma once

#include "raylib.h"
#include <vector>

class Tile
{
    public:
    Texture2D texture;
    int x;
    int y;
    Rectangle rect;
    Tile();
    Tile(Texture2D text);
    Tile(Texture2D text, int xPos, int yPos);
    void DrawTile();
    bool CheckForCollision(Rectangle otherRect);
};

Tile::Tile()
{

}

Tile::Tile(Texture2D text)
{
    texture = text;
}

Tile::Tile(Texture2D text, int xPos, int yPos)
{
    texture = text;
    x = xPos;
    y = yPos;
    rect = { (float)x, (float)y, 64, 64 };
}

void Tile::DrawTile()
{
    DrawTexture(texture, x, y, WHITE);
}

bool Tile::CheckForCollision(Rectangle otherRect)
{
    return CheckCollisionRecs(rect, otherRect);
}