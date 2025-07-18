#pragma once

#include "Tile.h"

class WallTile : public Tile
{
    public:
    WallTile();
    WallTile(Texture2D text, int xPos, int yPos);
};

WallTile::WallTile()
{

}

WallTile::WallTile(Texture2D text, int xPos, int yPos)
{
    texture = text;
    x = xPos;
    y = yPos;
    rect = { (float)x, (float)y, 64, 64 };
}
