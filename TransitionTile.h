#pragma once

#include "Tile.h"

class TransitionTile : public Tile
{
    public:
    int tileID;
    int destinationID;
    int playerDestX;
    int playerDestY;
    TransitionTile();
    TransitionTile(std::vector<Tile> tiles, int id, int xPos, int yPos, int destID, int pDestX, int pDestY);
};

TransitionTile::TransitionTile()
{

}

TransitionTile::TransitionTile(std::vector<Tile> tiles, int id, int xPos, int yPos, int destID, int pDestX, int pDestY)
{
    texture = tiles[id].texture;
    x = xPos;
    y = yPos;
    rect = { (float)x, (float)y, 64, 64 };
    tileID = id;
    destinationID = destID;
    playerDestX = pDestX;
    playerDestY = pDestY;
}

