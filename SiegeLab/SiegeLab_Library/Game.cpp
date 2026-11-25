#include "pch.h"
#include "framework.h"

#include "Game.h"
#include "InputController.h"
#include "Launcher.h"
#include "raylib.h"

void Game::Run()
{
    InitWindow(900, 600, "Test Launcher - SiegeLab");
    SetTargetFPS(60);

    siegelablib::Launcher launcher;
    siegelablib::InputController input;

    float angleX = 0.0f;
    float angleY = 30.0f;
    float speed = 16.0f;

    Camera3D camera = { 0 };
    camera.position = { -10.0f, 5.0f, 8.0f };
    camera.target = { 0.0f, 1.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ONE))
        {
            input.onKeyPressed(1, launcher);
        }
        if (IsKeyPressed(KEY_TWO))
        {
            input.onKeyPressed(2, launcher);
        }

        if (IsKeyDown(KEY_LEFT)) { angleX -= 30.0f * GetFrameTime(); }
        if (IsKeyDown(KEY_RIGHT)) { angleX += 30.0f * GetFrameTime(); }
        if (IsKeyDown(KEY_UP)) { angleY += 30.0f * GetFrameTime(); }
        if (IsKeyDown(KEY_DOWN)) { angleY -= 30.0f * GetFrameTime(); }

        input.onAimInput(angleX, angleY, speed, launcher);

        siegelablib::AimVec3 dir = launcher.getDirection();
        Vector3 start = { 0.0f, 1.0f, 0.0f };
        Vector3 end = {
            start.x + dir.x * 4.0f,
            start.y + dir.y * 4.0f,
            start.z + dir.z * 4.0f
        };

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawGrid(10, 1.0f);
        DrawSphere(start, 0.25f, RED);
        DrawLine3D(start, end, BLUE);
        EndMode3D();

        const char* kindText = (launcher.getKind() == siegelablib::ShotKind::Heavy)
            ? "Type: Heavy (2)"
            : "Type: Standard (1)";

        DrawText(kindText, 20, 20, 20, BLACK);
        DrawText(TextFormat("Angle X: %.1f | Angle Y: %.1f",
            launcher.getAngleXDeg(), launcher.getAngleYDeg()), 20, 50, 20, DARKGRAY);
        DrawText("Fleches: viser | Touches 1 / 2: type de tir", 20, 80, 18, GRAY);

        EndDrawing();
    }

    CloseWindow();
}
