#pragma once

#include "raylib.h"
#include "raymath.h"
#include "Projectile.h"

class Entity
{
    public:
    int health;
    Texture2D texture;
    int x;
    int y;
    int iFrames;
    Rectangle rect;
    Entity();
    Entity(Texture2D text, int xPos, int yPos);
    void DrawHitBox();
    void Draw();
    bool CheckForCollision(Rectangle otherRect);
    bool HasIFrames();
    float GetDegreeDirection(Vector2 pos, Vector2 otherPos, bool dontAdjustSecondPos);
    void LoseIFrames();
};

extern Projectile projSpawner;

Entity::Entity()
{
    
}

Entity::Entity(Texture2D text, int xPos, int yPos)
{
    texture = text;
    x = xPos;
    y = yPos;
    rect = { (float)x, (float)y, 64, 64 };
}

void Entity::Draw()
{
    if (iFrames % 8 == 7 || iFrames % 8 == 6) { return; }
    DrawTexture(texture, x, y, WHITE);
}

void Entity::DrawHitBox()
{
    DrawRectangleLinesEx(rect, 5, RED);
}

bool Entity::CheckForCollision(Rectangle otherRect)
{
    return CheckCollisionRecs(rect, otherRect);
}

bool Entity::HasIFrames()
{
    if (iFrames > 0)
    {
        return true;
    }

    return false;
}

float Entity::GetDegreeDirection(Vector2 pos, Vector2 otherPos, bool dontAdjustSecondPos)
{
    Vector2 adjustedPos;
    Vector2 adjustedOtherPos;

    adjustedPos = Vector2{pos.x + (float)32, pos.y + (float)32};
    if (dontAdjustSecondPos)
    {
        adjustedOtherPos = Vector2{otherPos.x, otherPos.y};
    }
    else
    {
        adjustedOtherPos = Vector2{otherPos.x + (float)32, otherPos.y + (float)32};
    }

    float dot = adjustedOtherPos.x - adjustedPos.x;
    float det = adjustedOtherPos.y - adjustedPos.y;
    float direction = atan2(det, dot);
    direction = (direction * 180) / PI;
    return direction;
}

void Entity::LoseIFrames()
{
    iFrames--;
}