/* Preparação para Etapa 10 - Exemplo 1: carregando e desenhando texturas */
#include "raylib.h"
#include "resource_dir.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Teste de Texturas");
    SetTargetFPS(60);

    /* procura a pasta "resources" e muda o diretório de trabalho para ela */
    SearchAndSetResourceDir("resources");

    /* só pode carregar texturas DEPOIS de InitWindow (precisa do OpenGL) */
    Texture2D ship = LoadTexture("ship.png");
    if (!IsTextureValid(ship)) {
        TraceLog(LOG_ERROR, "Não foi possível carregar ship.png");
        CloseWindow();
        return 1;
    }
    TraceLog(LOG_INFO, "Dimensões de ship.png: largura %d, altura %d",
             ship.width, ship.height);
    float rotation = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        rotation += 90.0f * dt; /* 90 graus por segundo */

        BeginDrawing();
        ClearBackground(BLACK);

        /* 1. posição do canto superior esquerdo, sem transformação */
        DrawTexture(ship, 40, 40, WHITE);

        /* 2. tint: multiplica cada pixel pela cor */
        DrawTexture(ship, 160, 40, RED);
        DrawTexture(ship, 280, 40, Fade(WHITE, 0.4f));

        /* 3. rotação (graus) e escala, em torno do canto superior esquerdo */
        DrawTextureEx(ship, (Vector2){450, 80}, rotation, 0.5f, WHITE);

        /* 4. só uma parte da textura (metade esquerda) */
        Rectangle half = {0, 0, ship.width / 2.0f, (float)ship.height};
        DrawTextureRec(ship, half, (Vector2){40, 200}, WHITE);

        /* 5. controle total: origem no centro, rotação e tamanho de destino */
        Rectangle src = {0, 0, (float)ship.width, (float)ship.height};
        Rectangle dst = {600, 300, ship.width * 1.5f, ship.height * 1.5f};
        Vector2 origin = {dst.width / 2, dst.height / 2};
        DrawTexturePro(ship, src, dst, origin, rotation, WHITE);

        DrawText(TextFormat("ship.png: %d x %d", ship.width, ship.height), 10,
                 SCREEN_HEIGHT - 30, 20, RAYWHITE);
        EndDrawing();
    }

    UnloadTexture(ship); /* libera a memória de vídeo */
    CloseWindow();
    return 0;
}
