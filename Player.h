#pragma once

#include "WallTile.h"
#include "Entity.h"

class Player : public Entity
{
    public:
    float speed;
    int coins;
    int arrowType;
    int reloadTimer;
    bool facingRight;
    Player();
    Player(Texture2D sprite, int xPos, int yPos);
    void PlayerControls();
    void ProcessPlayer();
};

extern bool isEditing;
extern std::vector<WallTile> loadedWallTiles;

Player::Player()
{

}

Player::Player(Texture2D text, int xPos, int yPos)
{
    texture = text;
    x = xPos;
    y = yPos;
    rect = { (float)x + 8, (float)y + 8, 48, 48 };
    speed = 3;
    iFrames = 0;
    coins = 0;
    arrowType = 0;
    reloadTimer = 0;
    health = 6;
    facingRight = true;
}

void Player::PlayerControls()
{
    if (IsKeyDown(KEY_W))
    {
        y -= speed;
        rect = { (float)x + 8, (float)y + 8, 48, 48 };

        for (WallTile tile : loadedWallTiles)
        {
            if (tile.CheckForCollision(rect))
            {
                y += speed;
                break;
            }
        }
    }
    if (IsKeyDown(KEY_S))
    {
        y += speed;
        rect = { (float)x + 8, (float)y + 8, 48, 48 };

        for (WallTile tile : loadedWallTiles)
        {
            if (tile.CheckForCollision(rect))
            {
                y -= speed;
                break;
            }
        }
    }
    if (IsKeyDown(KEY_A))
    {
        facingRight = false;
        x -= speed;
        rect = { (float)x + 8, (float)y + 8, 48, 48 };

        for (WallTile tile : loadedWallTiles)
        {
            if (tile.CheckForCollision(rect))
            {
                x += speed;
                break;
            }
        }
    }
    if (IsKeyDown(KEY_D))
    {
        facingRight = true;
        x += speed;
        rect = { (float)x + 8, (float)y + 8, 48, 48 };

        for (WallTile tile : loadedWallTiles)
        {
            if (tile.CheckForCollision(rect))
            {
                x -= speed;
                break;
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && reloadTimer <= 0 && !isEditing)
    {
        float direction = GetDegreeDirection(Vector2{(float)x, (float)y}, GetMousePosition(), true);
        projSpawner.CreateProjectile(arrowType, x, y, direction, true);
        reloadTimer = 50;
    }

    if (reloadTimer > 0) { reloadTimer--; }
}

void Player::ProcessPlayer()
{
    PlayerControls();
    Draw();

    if (iFrames > 0)
    {
        LoseIFrames();
    }
}
