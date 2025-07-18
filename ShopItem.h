#pragma once

#include "Player.h"
#include "Collectible.h"

class ShopItem : public Collectible
{
    public:
    int itemID;
    int price;
    int screenID;
    ShopItem();
    ShopItem(Texture2D text, int xPos, int yPos, int id, int buyPrice, int screen);
    void BuyItem();
    void Draw();
};

extern Player player;

ShopItem::ShopItem()
{

}

ShopItem::ShopItem(Texture2D text, int xPos, int yPos, int id, int buyPrice, int screen)
{
    texture = text;
    x = xPos;
    y = yPos;
    rect = { (float)x, (float)y, 64, 64 };
    itemID = id;
    price = buyPrice;
    screenID = screen;
}

void ShopItem::BuyItem()
{
    player.coins -= price;

    switch (itemID)
    {
    case 0:
        if (player.arrowType <= 0) { player.arrowType = 1; }
        break;
    case 1:
        if (player.arrowType <= 1) { player.arrowType = 2; }
    
    default:
        break;
    }
}

void ShopItem::Draw()
{
    DrawTexture(texture, x, y, WHITE);
    DrawText(TextFormat("%d", price), x + 20, y + 64, 20, WHITE);
    //DrawTextPro(GetFontDefault(), TextFormat("%d", price), Vector2{(float)x, (float)y}, Vector2{32, 32}, 0, 20, 2, WHITE);
}