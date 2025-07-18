#include <stdio.h>
#include <math.h>
#include <vector>
#include "Entity.h"
#include "Player.h"
#include "Coin.h"
#include "WallTile.h"
#include "EnemyPlant.h"
#include "Projectile.h"
#include <fstream>
#include <string.h>
#include "TransitionTile.h"
#include "ShopItem.h"
#include "EnemyCrab.h"
#include "Heart.h"
#include "EnemySpike.h"
#include "Boss.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 1000
#define TILE_MAP_WIDTH 22
#define TILE_MAP_HEIGHT 14
#define TILE_SIZE 64
#define ENEMY_I_FRAMES 40
#define PLAYER_I_FRAMES 90

Texture2D debugTexture;
Texture2D crosshair;
Texture2D crosshairInactive;
Texture2D heartFull;
Texture2D heartHalf;
Texture2D heartEmpty;
Texture2D playerRight;
Texture2D playerLeft;
Texture2D bossTexture;
Texture2D shopkeeper;

std::vector<Tile> tiles;
std::vector<Enemy> enemies;
std::vector<Projectile> projectiles;

int floorTileData[TILE_MAP_WIDTH * TILE_MAP_HEIGHT];
int wallTileData[TILE_MAP_WIDTH * TILE_MAP_HEIGHT];
int enemyData[TILE_MAP_WIDTH * TILE_MAP_HEIGHT];
std::vector<Tile> loadedFloorTiles;
std::vector<WallTile> loadedWallTiles;
std::vector<Tile> editorTiles;
std::vector<TransitionTile> loadedTransTiles;

Coin coinSpawner;
Heart heartSpawner;
Projectile projSpawner;

int screenXOffset = (SCREEN_WIDTH - (TILE_MAP_WIDTH * TILE_SIZE)) / 2;
int screenYOffset = (SCREEN_HEIGHT - (TILE_MAP_HEIGHT * TILE_SIZE));

Player player;
Boss boss;
std::vector<Coin> coinsLoaded;
std::vector<Heart> heartsLoaded;
std::vector<EnemyPlant> enemyPlants;
std::vector<EnemyCrab> enemyCrabs;
std::vector<EnemySpike> enemySpikes;

//std::vector<Projectile> loadedProjectiles;

std::vector<Projectile> projArrows;
std::vector<Projectile> projEnemies;

bool isEditing = false;
bool debugMode = false;
int mapNumber;
bool titleScreen = true;

int arrowType = 0;

Sound shootSFX;
Sound hitSFX;
Sound enemyShootSFX;
Sound coinSFX;

Music currentMusic;
Music mainMusic;
Music bossMusic;

// void DrawScreenTiles(int data[])
// {
//     int row = -1;
//     for (int i = 0; i < TILE_MAP_HEIGHT * TILE_MAP_WIDTH; i++)
//     {
//         if (i % TILE_MAP_WIDTH == 0) { row++; }
//         DrawTexture(tiles[data[i]], TILE_SIZE * (i % TILE_MAP_WIDTH) + screenXOffset, (row * TILE_SIZE) + screenYOffset, WHITE);
//     }
// }

void DrawDebugInfo()
{
    int tileHovering = 0;
    int tileHoveringX;
    int tileHoveringY;
    DrawText(TextFormat("%.f", GetMousePosition().x), 500, 50, 30, WHITE);
    DrawText(TextFormat("%.f", GetMousePosition().y), 500, 90, 30, WHITE);
    DrawText(TextFormat("%d", screenXOffset), 500, 130, 30, WHITE);
    DrawText(TextFormat("%d", screenYOffset), 500, 170, 30, WHITE);

    tileHoveringX = floor((GetMousePosition().x - screenXOffset) / TILE_SIZE);
    tileHoveringY = floor((GetMousePosition().y - screenYOffset) / TILE_SIZE);
    tileHovering = tileHoveringX + (TILE_MAP_WIDTH * tileHoveringY);

    DrawText(TextFormat("%d", tileHovering), 700, 50, 30, WHITE);
    DrawText(TextFormat("%d", tileHoveringX), 700, 130, 30, WHITE);
    DrawText(TextFormat("%d", tileHoveringY), 700, 170, 30, WHITE);

    DrawText(TextFormat("%d", mapNumber), 1500, 50, 30, WHITE);

    player.DrawHitBox();
    DrawText(TextFormat("%d", player.iFrames), player.x + 32, player.y - 16, 25, BLACK);

    for (Projectile arrow : projArrows)
    {
        arrow.DrawHitBox();
        DrawText(TextFormat("%d", arrow.id), arrow.x + 32, arrow.y - 64, 20, WHITE);
    }

    for (EnemyPlant enemy : enemyPlants)
    {
        enemy.DrawHitBox();

        DrawText(TextFormat("%d", enemy.id), enemy.x + 32, enemy.y + 70, 25, WHITE);
        if (enemy.HasIFrames())
        {
            DrawText(TextFormat("%d", enemy.iFrames), enemy.x + 32, enemy.y - 16, 25, BLACK);
        }
    }

    for (EnemyCrab enemy : enemyCrabs)
    {
        enemy.DrawHitBox();

        DrawText(TextFormat("%d", enemy.id), enemy.x + 32, enemy.y + 70, 25, WHITE);
        if (enemy.HasIFrames())
        {
            DrawText(TextFormat("%d", enemy.iFrames), enemy.x + 32, enemy.y - 16, 25, BLACK);
        }
    }

    player.health = 6;

    boss.DrawHitBox();
}

void LoadTileTextures()
{
    tiles.push_back(Tile(LoadTexture("resources/projectiles/arrow.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_ground.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_ground_leaves.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_area_exit.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_wall_horiz.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_wall_vert.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_wall_tl_corner.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_wall_tr_corner.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_wall_bl_corner.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_wall_br_corner.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_wall_vert_top.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_wall_vert_bottom.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_wall_horiz_left.png")));
    tiles.push_back(Tile(LoadTexture("resources/tiles/tile_wall_horiz_right.png")));
}

void LoadEnemies()
{
    enemies.push_back(Enemy(LoadTexture("resources/enemies/enemy_plant.png"), 6, 120, 1));
    enemies.push_back(Enemy(LoadTexture("resources/enemies/enemy_crab_rock.png"), 12, 90, 1));
    enemies.push_back(Enemy(LoadTexture("resources/enemies/enemy_spike.png"), 14, 0, 2));
    enemies.push_back(Enemy(LoadTexture("resources/enemies/enemy_spike.png"), 300, 100, 1));
}

void LoadProjectiles()
{
    projectiles.push_back(Projectile(LoadTexture("resources/projectiles/arrow.png"), 5, 0, 8, 2));
    projectiles.push_back(Projectile(LoadTexture("resources/projectiles/arrow_silver.png"), 8, 1, 10, 2));
    projectiles.push_back(Projectile(LoadTexture("resources/projectiles/arrow_cobalt.png"), 15, 3, 12, 2));
    projectiles.push_back(Projectile(LoadTexture("resources/projectiles/proj_plant.png"), 1, 0, 9, 0));
}

void CreateFloorTiles()
{
    loadedFloorTiles.clear();

    int tileX;
    int tileY;
    int row = -1;
    for (int i = 0; i < (int)(sizeof(floorTileData) / sizeof(floorTileData[0])); i++)
    {
        if (i % TILE_MAP_WIDTH == 0) { row++; }
        if (floorTileData[i] == 0) { continue; }
        
        tileX = TILE_SIZE * (i % TILE_MAP_WIDTH) + screenXOffset;
        tileY = (row * TILE_SIZE) + screenYOffset;
        loadedFloorTiles.push_back(Tile(tiles[floorTileData[i]].texture, tileX, tileY));
    }
}

void CreateWallTiles()
{
    loadedWallTiles.clear();

    int tileX;
    int tileY;
    int row = -1;
    for (int i = 0; i < (int)(sizeof(wallTileData) / sizeof(wallTileData[0])); i++)
    {
        if (i % TILE_MAP_WIDTH == 0) { row++; }
        if (wallTileData[i] == 0) { continue; }
        
        tileX = TILE_SIZE * (i % TILE_MAP_WIDTH) + screenXOffset;
        tileY = (row * TILE_SIZE) + screenYOffset;
        loadedWallTiles.push_back(WallTile(tiles[wallTileData[i]].texture, tileX, tileY));
    }
}

void CreateEnemies()
{
    enemyPlants.clear();
    enemyCrabs.clear();
    enemySpikes.clear();

    int enemX;
    int enemY;
    int row = -1;
    for (int i = 0; i < (int)(sizeof(enemyData) / sizeof(enemyData[0])); i++)
    {
        if (i % TILE_MAP_WIDTH == 0) { row++; }
        if (enemyData[i] == 0) { continue; }
        
        enemX = TILE_SIZE * (i % TILE_MAP_WIDTH) + screenXOffset;
        enemY = (row * TILE_SIZE) + screenYOffset;

        if (enemyData[i] == 1)
        {
            enemyPlants.push_back(EnemyPlant(enemies[enemyData[i] - 1], enemX, enemY, enemyPlants.size()));
        }
        else if (enemyData[i] == 2)
        {
            enemyCrabs.push_back(EnemyCrab(enemies[enemyData[i] - 1], enemX, enemY, enemyCrabs.size()));
        }
        else if (enemyData[i] == 3)
        {
            enemySpikes.push_back(EnemySpike(enemies[enemyData[i] - 1], enemX, enemY, enemySpikes.size()));
        }
    }
}

void AddTransitionTileFromEditor(int tileID, int tileHovered, int dest)
{
    int x;
    int y;
    int i = tileHovered;
    int row = 0;
    while (i >= 22)
    {
        i -= 22;
        row++;
    }
    
    x = TILE_SIZE * (tileHovered % TILE_MAP_WIDTH) + screenXOffset;
    y = (row * TILE_SIZE) + screenYOffset;

    if (tileID == 0)
    {
        for (int i = 0; i < (int)loadedTransTiles.size();)
        {
            if (x == loadedTransTiles[i].x && y == loadedTransTiles[i].y)
            {
                loadedTransTiles.erase(loadedTransTiles.begin() + i);
                return;
            }
            else
            {
                i++;
            }
        }

        return;
    }
    
    for (TransitionTile tile : loadedTransTiles)
    {
        if (x == tile.x && y == tile.y)
        {
            return;
        }
    }

    loadedTransTiles.push_back(TransitionTile(tiles, tileID, x, y, dest, 0, 0));
}

void AddTransitionTile(int tileID, int x, int y, int dest, int playerDestX, int playerDestY)
{
    loadedTransTiles.push_back(TransitionTile(tiles, tileID, x, y, dest, playerDestX, playerDestY));
}

void LoadMapFromFile(int mapID)
{
    projArrows.clear();
    projEnemies.clear();
    coinsLoaded.clear();
    heartsLoaded.clear();

    std::ifstream fileIn;
    std::string path = "maps/map" + std::to_string(mapID) + ".txt";

    loadedTransTiles.clear();

    fileIn.open(path);

    for (int i = 0; i < TILE_MAP_HEIGHT * TILE_MAP_WIDTH; i++)
    {
        fileIn >> floorTileData[i];
    }
    for (int i = 0; i < TILE_MAP_HEIGHT * TILE_MAP_WIDTH; i++)
    {
        fileIn >> wallTileData[i];
    }
    for (int i = 0; i < TILE_MAP_HEIGHT * TILE_MAP_WIDTH; i++)
    {
        fileIn >> enemyData[i];
    }
    int transTileAmount;
    fileIn >> transTileAmount;

    int tileID, x, y, dest, playerDestX, playerDestY;

    for (int i = 0; i < transTileAmount; i++)
    {
        fileIn >> tileID;
        fileIn >> x >> y;
        fileIn >> dest;
        fileIn >> playerDestX;
        fileIn >> playerDestY;
        AddTransitionTile(tileID, x, y, dest, playerDestX, playerDestY);
    }

    CreateFloorTiles();
    CreateWallTiles();
    CreateEnemies();

    fileIn.close();

    if (mapID == 16)
    {
        //boss.health = enemies[3].health;
        boss = Boss(enemies[3], 750, 400, 0);
        currentMusic = bossMusic;
        PlayMusicStream(currentMusic);
    }
    if (mapID == 0)
    {
        currentMusic = mainMusic;
        PlayMusicStream(currentMusic);
    }
}

void CreateMapFile(int mapID)
{
    std::ofstream fileOut;
    std::string path = "maps/map" + std::to_string(mapID) + ".txt";

    fileOut.open(path);

    for (int i = 0; i < TILE_MAP_HEIGHT * TILE_MAP_WIDTH; i++)
    {
        if (i % TILE_MAP_WIDTH == 0 && i != 0)
        {
            fileOut << std::endl;
        }
        fileOut << floorTileData[i] << " ";
    }
    fileOut << std::endl;
    fileOut << std::endl;
    for (int i = 0; i < TILE_MAP_HEIGHT * TILE_MAP_WIDTH; i++)
    {
        if (i % TILE_MAP_WIDTH == 0 && i != 0)
        {
            fileOut << std::endl;
        }

        fileOut << wallTileData[i] << " ";;
    }
    fileOut << std::endl;
    fileOut << std::endl;
    for (int i = 0; i < TILE_MAP_HEIGHT * TILE_MAP_WIDTH; i++)
    {
        if (i % TILE_MAP_WIDTH == 0 && i != 0)
        {
            fileOut << std::endl;
        }

        fileOut << enemyData[i] << " ";
    }
    fileOut << std::endl;
    fileOut << std::endl;
    fileOut << loadedTransTiles.size() << std::endl;
    for (int i = 0; i < (int)loadedTransTiles.size(); i++)
    {
        fileOut << loadedTransTiles[i].tileID << " ";
        fileOut << loadedTransTiles[i].x << " ";
        fileOut << loadedTransTiles[i].y << " ";
        fileOut << loadedTransTiles[i].destinationID << " ";
        fileOut << loadedTransTiles[i].playerDestX << " ";
        fileOut << loadedTransTiles[i].playerDestY << std::endl;
    }
    
    fileOut.close();
}

void TransitionMovePlayer(int x, int y)
{
    player.x = x;
    player.y = y;
    player.rect = { (float)x, (float)y, 64, 64 };
}

void DrawHearts()
{
    if (player.health >= 2)
    {
        DrawTexture(heartFull, 1200, 30, WHITE);
    }
    else if (player.health == 1)
    {
        DrawTexture(heartHalf, 1200, 30, WHITE);
    }
    else
    {
        DrawTexture(heartEmpty, 1200, 30, WHITE);
    }

    if (player.health >= 4)
    {
        DrawTexture(heartFull, 1270, 30, WHITE);
    }
    else if (player.health == 3)
    {
        DrawTexture(heartHalf, 1270, 30, WHITE);
    }
    else
    {
        DrawTexture(heartEmpty, 1270, 30, WHITE);
    }

    if (player.health >= 6)
    {
        DrawTexture(heartFull, 1340, 30, WHITE);
    }
    else if (player.health == 5)
    {
        DrawTexture(heartHalf, 1340, 30, WHITE);
    }
    else
    {
        DrawTexture(heartEmpty, 1340, 30, WHITE);
    }
}

int main()
{
    SetTargetFPS(60);

    mapNumber = 0;

    int tileHovering;
    int tileHoveringX;
    int tileHoveringY;
    int tileSelected = 0;

    int editorTileType = 0;
    // 0 : Floor
    // 1 : Wall
    // 2 : Event

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Final Project :D");

    SetRandomSeed((unsigned int)time(NULL));

    InitAudioDevice();

    mainMusic = LoadMusicStream("resources/Main Song.wav");
    bossMusic = LoadMusicStream("resources/Boss Music.mp3");
    currentMusic = mainMusic;

    shootSFX = LoadSound("resources/shoot.wav");
    hitSFX = LoadSound("resources/hit.wav");
    enemyShootSFX = LoadSound("resources/enemyShoot.wav");
    coinSFX = LoadSound("resources/pickupCoin.wav");

    LoadTileTextures();
    LoadEnemies();
    LoadProjectiles();

    debugTexture = LoadTexture("resources/projectiles/arrow.png");
    crosshair = LoadTexture("resources/crosshair.png");
    crosshairInactive = LoadTexture("resources/crosshair_red.png");
    heartFull = LoadTexture("resources/heart_full.png");
    heartHalf = LoadTexture("resources/heart_half.png");
    heartEmpty = LoadTexture("resources/heart_empty.png");
    playerRight = LoadTexture("resources/player_right.png");
    playerLeft = LoadTexture("resources/player_left.png");
    shopkeeper = LoadTexture("resources/shopkeeper.png");

    editorTiles.emplace_back(tiles[0].texture, 20, 150);
    editorTiles.emplace_back(tiles[1].texture, 20, 220);
    editorTiles.emplace_back(tiles[2].texture, 20, 290);
    editorTiles.emplace_back(tiles[3].texture, 1516, 150);
    editorTiles.emplace_back(tiles[4].texture, 1516, 220);
    editorTiles.emplace_back(tiles[5].texture, 1516, 290);
    editorTiles.emplace_back(tiles[6].texture, 1516, 360);
    editorTiles.emplace_back(tiles[7].texture, 1516, 430);
    editorTiles.emplace_back(tiles[8].texture, 1516, 500);
    editorTiles.emplace_back(tiles[9].texture, 1516, 570);
    editorTiles.emplace_back(tiles[10].texture, 1516, 640);
    editorTiles.emplace_back(tiles[11].texture, 1516, 710);
    editorTiles.emplace_back(tiles[12].texture, 1516, 780);
    editorTiles.emplace_back(tiles[13].texture, 1516, 850);

    coinSpawner.texture = LoadTexture("resources/coin.png");
    heartSpawner.texture = LoadTexture("resources/heart_full.png");

    player = Player(playerRight, 768, 500);

    boss = Boss(enemies[3], 750, 400, 0);

    ShopItem silverArrowUpgrade = ShopItem(projectiles[1].texture, 500, 360, 0, 50, 5);
    ShopItem cobaltArrowUpgrade = ShopItem(projectiles[2].texture, 500, 360, 1, 110, 10);

    LoadMapFromFile(mapNumber);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_P))
        {
            if (isEditing)
            {
                isEditing = false;
            }
            else
            {
                //isEditing = true;
            }
        }

        if (IsKeyPressed(KEY_Q))
        {
            if (debugMode)
            {
                debugMode = false;
            }
            else
            {
                //debugMode = true;
            }
        }

        if (IsKeyPressed(KEY_ONE))
        {
            editorTileType = 0;
        }
        else if (IsKeyPressed(KEY_TWO))
        {
            editorTileType = 1;
        }
        else if (IsKeyPressed(KEY_THREE))
        {
            editorTileType = 2;
        }
        else if (IsKeyPressed(KEY_FOUR))
        {
            editorTileType = 3;
        }

        BeginDrawing();

        if (titleScreen)
        {
            DrawText("The Tale of", 500, 200, 100, WHITE);
            DrawText("Bow Guy", 400, 300, 200, RED);
            DrawText("Press \"T\" to begin!", 500, 920, 60, WHITE);
            DrawTextureEx(playerRight, Vector2 {620, 500}, 0, 6, WHITE);
            ClearBackground(Color{16, 23, 80});
            if (IsKeyDown(KEY_T))
            {
                titleScreen = false;
            }

            EndDrawing();
            continue;
        }

        ClearBackground(DARKBLUE);
        UpdateMusicStream(currentMusic);

        for (Tile tile : loadedFloorTiles)
        {
            tile.DrawTile();
        }

        for (WallTile tile : loadedWallTiles)
        {
            tile.DrawTile();

            for (Projectile arrow : projArrows)
            {
                if (tile.CheckForCollision(arrow.rect))
                {
                    arrow.Destroy(projArrows);
                }
            }

            for (Projectile proj : projEnemies)
            {
                if (tile.CheckForCollision(proj.rect))
                {
                    proj.Destroy(projEnemies);
                }
            }
        }

        for (TransitionTile tile : loadedTransTiles)
        {
            tile.DrawTile();
            
            if (tile.CheckForCollision(player.rect))
            {
                mapNumber = tile.destinationID;
                LoadMapFromFile(mapNumber);
                TransitionMovePlayer(tile.playerDestX, tile.playerDestY);
                break;
            }

            for (Projectile arrow : projArrows)
            {
                if (tile.CheckForCollision(arrow.rect))
                {
                    arrow.Destroy(projArrows);
                }
            }

            for (Projectile proj : projEnemies)
            {
                if (tile.CheckForCollision(proj.rect))
                {
                    proj.Destroy(projEnemies);
                }
            }
        }

        for (EnemyPlant &enem : enemyPlants)
        {
            enem.Draw();
            enem.AttemptAttack();

            if (enem.HasIFrames())
            {
                enem.LoseIFrames();
                continue;
            }

            for (Projectile &arrow : projArrows)
            {
                if (enem.CheckForCollision(arrow.rect))
                {
                    PlaySound(hitSFX);
                    enem.iFrames = ENEMY_I_FRAMES;
                    enem.health -= arrow.damage;

                    arrow.pierce--;
                    if (arrow.pierce < 0) { arrow.Destroy(projArrows); }
                }
            }

            if (enem.health <= 0)
            {
                enem.Die(enemyPlants);
            }
        }

        for (EnemyCrab &enem : enemyCrabs)
        {
            enem.Draw();
            enem.AttemptAttack();

            for (WallTile tile : loadedWallTiles)
            {
                if (tile.CheckForCollision(enem.rect))
                {
                    enem.dashTimer = 0;
                    enem.direction += 180;
                    enem.Dashing();
                    enem.Dashing();
                }
            }

            if (!enem.HasIFrames())
            {
                for (Projectile &arrow : projArrows)
                {
                    if (enem.CheckForCollision(arrow.rect))
                    {
                        PlaySound(hitSFX);
                        enem.iFrames = ENEMY_I_FRAMES;
                        enem.health -= arrow.damage;

                        arrow.pierce--;
                        if (arrow.pierce < 0) { arrow.Destroy(projArrows); }
                    }
                }
            }
            else
            {
                enem.LoseIFrames();
            }

            if (enem.health <= 0)
            {
                enem.Die(enemyCrabs);
            }

            if (player.HasIFrames()) { continue; }

            if (player.CheckForCollision(enem.rect))
            {
                PlaySound(hitSFX);
                player.iFrames = PLAYER_I_FRAMES;
                player.health -= enem.contactDamage;
            }
        }

        for (EnemySpike &enem : enemySpikes)
        {
            enem.Draw();
            enem.Move();

            for (WallTile tile : loadedWallTiles)
            {
                if (tile.CheckForCollision(enem.rect))
                {
                    enem.direction += 180;
                    enem.Move();
                    enem.Move();
                    if (GetRandomValue(0, 1) == 0)
                    {
                        enem.direction += 90;
                    }
                    else
                    {
                        enem.direction -= 90;
                    }
                }
            }

            for (TransitionTile tile : loadedTransTiles)
            {
                if (tile.CheckForCollision(enem.rect))
                {
                    enem.direction += 180;
                    enem.Move();
                    enem.Move();
                    if (GetRandomValue(0, 1) == 0)
                    {
                        enem.direction += 90;
                    }
                    else
                    {
                        enem.direction -= 90;
                    }
                }
            }

            if (!enem.HasIFrames())
            {
                for (Projectile &arrow : projArrows)
                {
                    if (enem.CheckForCollision(arrow.rect))
                    {
                        PlaySound(hitSFX);
                        enem.iFrames = ENEMY_I_FRAMES;
                        enem.health -= arrow.damage;

                        arrow.pierce--;
                        if (arrow.pierce < 0) { arrow.Destroy(projArrows); }
                    }
                }
            }
            else
            {
                enem.LoseIFrames();
            }

            if (enem.health <= 0)
            {
                enem.Die(enemySpikes);
            }

            if (player.HasIFrames()) { continue; }

            if (player.CheckForCollision(enem.rect))
            {
                PlaySound(hitSFX);
                player.iFrames = PLAYER_I_FRAMES;
                player.health -= enem.contactDamage;
            }
        }

        for (Projectile &arrow : projArrows)
        {
            arrow.Draw();
            arrow.GoForward();
        }

        for (Projectile &proj : projEnemies)
        {
            proj.Draw();
            proj.GoForward();

            if (player.HasIFrames()) { continue; }
            
            if (player.CheckForCollision(proj.rect))
            {
                PlaySound(hitSFX);
                player.iFrames = PLAYER_I_FRAMES;
                player.health -= proj.damage;

                proj.pierce--;
                if (proj.pierce < 0) { proj.Destroy(projEnemies); }
            }
        }

        if (silverArrowUpgrade.screenID == mapNumber && player.arrowType == 0)
        {
            silverArrowUpgrade.Draw();
            if (silverArrowUpgrade.CheckForCollision(player) &&  silverArrowUpgrade.price <= player.coins)
            {
                silverArrowUpgrade.BuyItem();
            }
        }

        if (cobaltArrowUpgrade.screenID == mapNumber && player.arrowType >= 2)
        {
            cobaltArrowUpgrade.Draw();
            if (cobaltArrowUpgrade.CheckForCollision(player) && cobaltArrowUpgrade.price <= player.coins)
            {
                cobaltArrowUpgrade.BuyItem();
            }
        }

        if (mapNumber == 5 || mapNumber == 10)
        {
            DrawTexture(shopkeeper, 768, 200, WHITE);
            DrawText("Buy something with your coins!", 500, 280, 40, WHITE);
        }

        if (mapNumber == 16)
        {
            if (!boss.dead)
            {
                boss.Draw();

                if (boss.active)
                {
                    boss.RunAI();
                    boss.DrawHealthBar();
                }
                else
                {
                    if (boss.introWaitTimer > 0)
                    {
                        boss.introWaitTimer--;
                    }
                    else if (boss.healthBarTimer < boss.health)
                    {
                        boss.FillHealthBar();
                    }
                    else
                    {
                        boss.active = true;
                    }
                }

                if (!boss.HasIFrames())
                {
                    for (Projectile &arrow : projArrows)
                    {
                        if (boss.CheckForCollision(arrow.rect))
                        {
                            PlaySound(hitSFX);
                            boss.iFrames = ENEMY_I_FRAMES;
                            boss.health -= arrow.damage;

                            arrow.pierce--;
                            if (arrow.pierce < 0) { arrow.Destroy(projArrows); }
                        }
                    }
                }
                else
                {
                    boss.LoseIFrames();
                }

                if (!player.HasIFrames())
                {
                    if (player.CheckForCollision(boss.rect))
                    {
                        PlaySound(hitSFX);
                        player.iFrames = PLAYER_I_FRAMES;
                        player.health -= boss.contactDamage;
                    }
                }
            }
        }

        DrawText(TextFormat("Coins : %d", player.coins), 100, 40, 50, WHITE);
        DrawHearts();

        if (isEditing)
        {
            DrawText("Editing", 0, 0, 50, RED);
            switch (editorTileType)
            {
                case 0:
                    DrawText("Floor", 10, 50, 30, RED);
                    break;
                case 1:
                    DrawText("Wall", 10, 50, 30, RED);
                    break;
                case 2:
                    DrawText("Transition", 10, 50, 30, RED);
                    break;
                case 3:
                    DrawText("Enemies", 10, 50, 30, RED);
                    break;
                case 4:
                    DrawText("Event", 10, 50, 30, RED);
                default:
                    DrawText("Invalid Type", 10, 50, 30, RED);
            }

            for (Tile tile : editorTiles)
            {
                tile.DrawTile();
            }
        }

        if (isEditing)
        {
            DrawTexture(tiles[tileSelected].texture, (GetMousePosition().x - (TILE_SIZE / 2)), GetMousePosition().y - (TILE_SIZE / 2), WHITE);
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                for (int i = 0; i < (int)editorTiles.size(); i++)
                {
                    if (CheckCollisionPointRec(GetMousePosition(), editorTiles[i].rect))
                    {
                        tileSelected = i;
                    }
                }

                if (GetMousePosition().x > (SCREEN_WIDTH - (TILE_MAP_WIDTH * TILE_SIZE)) / 2 && GetMousePosition().x < SCREEN_WIDTH - screenXOffset)
                {
                    if (GetMousePosition().y > (SCREEN_HEIGHT - (TILE_MAP_HEIGHT * TILE_SIZE)))
                    {
                        tileHoveringX = floor((GetMousePosition().x - screenXOffset) / TILE_SIZE);
                        tileHoveringY = floor((GetMousePosition().y - screenYOffset) / TILE_SIZE);
                        tileHovering = tileHoveringX + (TILE_MAP_WIDTH * tileHoveringY);

                        if (editorTileType == 0)
                        {
                            floorTileData[tileHovering] = tileSelected;
                            CreateFloorTiles();
                        }
                        else if (editorTileType == 1)
                        {
                            wallTileData[tileHovering] = tileSelected;
                            CreateWallTiles();
                        }
                        else if (editorTileType == 2)
                        {
                            AddTransitionTileFromEditor(tileSelected, tileHovering, 0);
                        }
                        else if (editorTileType == 3)
                        {
                            enemyData[tileHovering] = tileSelected;
                            CreateEnemies();
                        }
                    }
                }
            }

            if (IsKeyPressed(KEY_N))
            {
                if (debugMode)
                {
                    //CreateMapFile(mapNumber);
                }
            }
        }

        if (debugMode)
        {
            if (IsKeyPressed(KEY_M))
            {
                LoadMapFromFile(15);
            }
        }

        player.ProcessPlayer();

        if (player.facingRight)
        {
            player.texture = playerRight;
        }
        else
        {
            player.texture = playerLeft;
        }

        if (debugMode)
        {
            DrawDebugInfo();
        }

        for (Coin coin : coinsLoaded)
        {
            coin.Draw();
            if (coin.CheckForCollision(player))
            {
                PlaySound(coinSFX);
                coin.CollectCoin(player);
                coin.Destroy(coinsLoaded);
            }

            if (debugMode)
            {
                DrawText(TextFormat("%d", coin.id), coin.x + 32, coin.y + 70, 25, WHITE);
            }
        }

        for (Heart heart : heartsLoaded)
        {
            heart.Draw();
            if (heart.CheckForCollision(player))
            {
                heart.CollectHeart(player);
                heart.Destroy(heartsLoaded);
            }

            if (debugMode)
            {
                DrawText(TextFormat("%d", heart.id), heart.x + 32, heart.y + 70, 25, WHITE);
            }
        }

        if (player.reloadTimer > 0)
        {
            DrawTexture(crosshairInactive, GetMousePosition().x - 32, GetMousePosition().y - 32, WHITE);
        }
        else
        {
            DrawTexture(crosshair, GetMousePosition().x - 32, GetMousePosition().y - 32, WHITE);
        }

        if (player.health <= 0)
        {
            LoadMapFromFile(0);
            player.x = 768;
            player.y = 500;
            mapNumber = 0;
            player.coins -= 10;
            player.health = 6;

            if (player.coins < 0)
            {
                player.coins = 0;
            }
        }

        if (mapNumber == 16 && boss.dead)
        {
            projEnemies.clear();
            projArrows.clear();
            DrawRectangle(360, 230, 900, 600, BLACK);
            DrawText("You won!", 610, 400, 100, WHITE);
            DrawText("Click R to go back to the\nwith your upgrades!", 430, 520, 60, WHITE);
            if (IsKeyDown(KEY_R))
            {
                LoadMapFromFile(0);
                mapNumber = 0;
            }
        }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
