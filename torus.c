#include "raylib.h"
#include "raymath.h"

int main(void)
{
    InitWindow(800, 600, "Raylib 5.5 - Torus Example");

    Camera camera = {
        .position = (Vector3){ 5.0f, 5.0f, 5.0f },
        .target = (Vector3){ 0.0f, 0.0f, 0.0f },
        .up = (Vector3){ 0.0f, 1.0f, 0.0f },
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE
    };

    Mesh torusMesh = GenMeshTorus(0.1f, 10.0f, 32, 16);  // Major radius, minor radius, segments
    Model torusModel = LoadModelFromMesh(torusMesh);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawModel(torusModel, Vector3Zero(), 1.0f, RED);
                DrawGrid(20, 1.0f);
            EndMode3D();
            DrawText("Torus created with GenMeshTorus", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    UnloadModel(torusModel);
    CloseWindow();
    return 0;
}
