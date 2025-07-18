#pragma once

#include "raylib.h"

class Projectile
{
    public:
    Texture2D texture;
    int damage;
    int pierce;
    float speed;
    int rotationType;
    float x;
    float y;
    float direction;
    int id;
    Rectangle rect;
    Projectile();
    Projectile(Texture2D text, int atk, int pier, float spd, int rotType);
    Projectile(Projectile proj, int xPos, int yPos, float dir, int num);
    void Draw();
    void GoForward();
    void DrawHitBox();
    void Destroy(std::vector<Projectile> &items);
    void CreateProjectile(int type, float x, float y, float direction, bool isPlayerProj);
};

extern std::vector<Projectile> projectiles;
extern std::vector<Projectile> projArrows;
extern std::vector<Projectile> projEnemies;

extern Sound shootSFX;
extern Sound enemyShootSFX;

Projectile::Projectile()
{
    
}

Projectile::Projectile(Texture2D text, int atk, int pier, float spd, int rotType)
{
    texture = text;
    pierce = pier;
    damage = atk;
    speed = spd;
    rotationType = rotType;
}

Projectile::Projectile(Projectile proj, int xPos, int yPos, float dir, int num) : Projectile(proj)
{
    x = xPos;
    y = yPos;
    direction = dir;
    id = num;
    rect = { x - 24, y - 24, 48, 48 };
}

void Projectile::Draw()
{
    switch (rotationType)
    {
    case 1:
        DrawTexturePro(texture, Rectangle{0, 0, 64, 64}, Rectangle{x, y, 64, 64}, Vector2{32, 32}, direction, WHITE);
        break;
    case 2:
        DrawTexturePro(texture, Rectangle{0, 0, 64, 64}, Rectangle{x, y, 64, 64}, Vector2{32, 32}, direction + 45, WHITE);
        break;
    default:
        DrawTexturePro(texture, Rectangle{0, 0, 64, 64}, Rectangle{x, y, 64, 64}, Vector2{32, 32}, 0, WHITE);
    }
}

void Projectile::GoForward()
{
    x += cos((direction * PI) / 180) * speed;
    y += sin((direction * PI) / 180) * speed;
    rect = { x - 24, y - 24, 48, 48 };
}

void Projectile::DrawHitBox()
{
    DrawRectangleLinesEx(rect, 5, WHITE);
}

void Projectile::CreateProjectile(int type, float xPos, float yPos, float direction, bool isPlayerProj)
{
    if (isPlayerProj)
    {
        PlaySound(shootSFX);
        projArrows.push_back(Projectile(projectiles[type], xPos + 32, yPos + 32, direction, projArrows.size()));
    }
    else
    {
        PlaySound(enemyShootSFX);
        projEnemies.push_back(Projectile(projectiles[type], xPos + 32, yPos + 32, direction, projEnemies.size()));
    }
}

void Projectile::Destroy(std::vector<Projectile> &arrows)
{
    int oldID = id;

    arrows.erase(arrows.begin() + id);

    for (int i = oldID; i < (float)arrows.size(); i++)
    {
        arrows[i].id -= 1;
    }
}
