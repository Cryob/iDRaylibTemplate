#pragma once

#include "Tile.h"

class EditorTile : Tile
{
    public:
    Texture2D sprite;
    int x;
    int y;
    Rectangle rect;
    EditorTile(Texture2D text, int xPos, int yPos);
};

EditorTile::EditorTile(Texture2D text, int xPos, int yPos)
{
    sprite = text;
    x = xPos;
    y = yPos;
    rect = { (float)x, (float)y, 64, 64 };
}
