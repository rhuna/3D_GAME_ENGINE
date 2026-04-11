#include "raylib.h"
#include "game/scenes/OpenWorldFoundationScene.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, "3D_GAME_ENGINE v25 Combat Rewards + Enemy Variety + Progression Loop");
    SetTargetFPS(60);

    fw::OpenWorldFoundationScene scene;
    scene.Load();

    while (!WindowShouldClose())
    {
        scene.Update(GetFrameTime());
        BeginDrawing();
        ClearBackground({24, 28, 36, 255});
        scene.Draw();
        EndDrawing();
    }

    scene.Unload();
    CloseWindow();
    return 0;
}
