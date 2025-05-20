#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <math.h>

#define TORUS_MAJOR_RADIUS 2.0f
#define TORUS_MINOR_RADIUS 0.5f
#define TORUS_MAJOR_SEGMENTS 64
#define TORUS_MINOR_SEGMENTS 32

Mesh GenTorusMesh(float R, float r, int rings, int sides) {
    int vertexCount = rings * sides * 6;
    Vector3 *vertices = (Vector3 *)MemAlloc(vertexCount * sizeof(Vector3));
    Vector3 *normals = (Vector3 *)MemAlloc(vertexCount * sizeof(Vector3));
    Vector2 *texcoords = (Vector2 *)MemAlloc(vertexCount * sizeof(Vector2));

    int index = 0;
    for (int i = 0; i < rings; i++) {
        float theta1 = ((float)i / rings) * 2.0f * PI;
        float theta2 = ((float)(i + 1) / rings) * 2.0f * PI;

        for (int j = 0; j < sides; j++) {
            float phi1 = ((float)j / sides) * 2.0f * PI;
            float phi2 = ((float)(j + 1) / sides) * 2.0f * PI;

            Vector3 p[4];
            Vector3 n[4];
            Vector2 t[4];

            for (int k = 0; k < 4; k++) {
                float theta = (k < 2) ? theta1 : theta2;
                float phi   = (k % 2 == 0) ? phi1 : phi2;

                float cosTheta = cosf(theta), sinTheta = sinf(theta);
                float cosPhi   = cosf(phi),   sinPhi   = sinf(phi);

                float x = (R + r * cosPhi) * cosTheta;
                float y = r * sinPhi;
                float z = (R + r * cosPhi) * sinTheta;

                float nx = cosPhi * cosTheta;
                float ny = sinPhi;
                float nz = cosPhi * sinTheta;

                p[k] = (Vector3){ x, y, z };
                n[k] = (Vector3){ nx, ny, nz };
                t[k] = (Vector2){ (float)i / rings, (float)j / sides };
            }

            // Two triangles per quad
            vertices[index] = p[0]; normals[index] = n[0]; texcoords[index++] = t[0];
            vertices[index] = p[1]; normals[index] = n[1]; texcoords[index++] = t[1];
            vertices[index] = p[2]; normals[index] = n[2]; texcoords[index++] = t[2];

            vertices[index] = p[2]; normals[index] = n[2]; texcoords[index++] = t[2];
            vertices[index] = p[1]; normals[index] = n[1]; texcoords[index++] = t[1];
            vertices[index] = p[3]; normals[index] = n[3]; texcoords[index++] = t[3];
        }
    }

    Mesh mesh = { 0 };
    mesh.vertexCount = vertexCount;
    mesh.triangleCount = vertexCount / 3;
    mesh.vertices = (float *)vertices;
    mesh.normals = (float *)normals;
    mesh.texcoords = (float *)texcoords;

    UploadMesh(&mesh, false);
    return mesh;
}

int main(void)
{
    InitWindow(800, 600, "Raylib - Torus");
    Camera camera = { 0 };
    camera.position = (Vector3){ 6.0f, 6.0f, 6.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Mesh torus = GenTorusMesh(TORUS_MAJOR_RADIUS, TORUS_MINOR_RADIUS, TORUS_MAJOR_SEGMENTS, TORUS_MINOR_SEGMENTS);
    Model model = LoadModelFromMesh(torus);

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);

                DrawModel(model, Vector3Zero(), 1.0f, RED);
                DrawGrid(20, 1.0f);

            EndMode3D();
            DrawText("Torus rendered with Raylib", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    UnloadModel(model);
    CloseWindow();
    return 0;
}