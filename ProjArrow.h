#pragma once

#include "Projectile.h"

class ProjArrow : public Projectile
{
    public:
    ProjArrow();
    ProjArrow(Projectile proj, int xPos, int yPos, float dir, int num);
    void Destroy(std::vector<ProjArrow> &arrows);
    void CreateProjectile(int x, int y, float direction);
};

ProjArrow::ProjArrow()
{

}

ProjArrow::ProjArrow(Projectile proj, int xPos, int yPos, float dir, int num) : Projectile(proj)
{
    x = xPos;
    y = yPos;
    direction = dir;
    id = num;
    rect = { x - 24, y - 24, 48, 48 };
}

void ProjArrow::Destroy(std::vector<ProjArrow> &arrows)
{
    arrows.erase(arrows.begin() + id);

    for (int i = id; i < (int)arrows.size(); i++)
    {
        arrows[i].id -= 1;
    }
}
