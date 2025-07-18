#include "raylib.h"
#pragma GCC diagnostic ignored "-Waddress"

int main()
{
    int screenWidth = 800;
    int screenHeight = 600;
    const char* emotion = { "array" };
    Color bgColor = {22, 30, 96, 255};

    double timePlayed = 0.0;
    double timeSincePlay = 0;
    double timeToAngry = 10;

    InitWindow(screenWidth, screenHeight, "Raylib");

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        timeSincePlay = GetTime() - timePlayed;

        BeginDrawing();

        ClearBackground(bgColor);
        DrawText("RoboGuy", 0, 0, 50, RED);

        //DrawCircle(screenHeight / 2, screenWidth / 2, 100, YELLOW);
        //DrawCircle((screenHeight / 2) + 40, (screenWidth / 2) - 30, 15, BLACK);
        //DrawCircle((screenHeight / 2) - 40, (screenWidth / 2) - 30, 15, BLACK);
        DrawRectangle((screenWidth / 2) - 100, (screenHeight / 2) - 100, 200, 200, GRAY);

        if (IsKeyDown(KEY_P))
        {
            DrawText("Playing!", 550, 100, 50, RED);
            timePlayed = GetTime();
        }

        if (timeSincePlay >= timeToAngry)
        {
            emotion = "angry";
        }
        else
        {
            emotion = "happy";
        }

        if (emotion == "happy")
        {
            DrawCircle((screenWidth / 2) - 45, (screenHeight / 2) - 40, 25, YELLOW);
            DrawCircle((screenWidth / 2) + 45, (screenHeight / 2) - 40, 25, YELLOW);
            DrawRectangle((screenWidth / 2) - 75, (screenHeight / 2) + 25, 150, 35, YELLOW);
            DrawText("RoboGuy is happy!", 250, 450, 30, RED);
        }
        else
        {
            DrawCircle((screenWidth / 2) - 45, (screenHeight / 2) - 40, 25, RED);
            DrawCircle((screenWidth / 2) + 45, (screenHeight / 2) - 40, 25, RED);
            DrawRectangle((screenWidth / 2) - 75, (screenHeight / 2) + 25, 150, 35, RED);
            DrawText("RoboGuy is not happy with you", 150, 450, 30, RED);
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}