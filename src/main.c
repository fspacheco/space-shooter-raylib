#include "objects.h" /* definições das structs */
#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400

/* protótipos */
void init_ship(SpaceShip *ship);
void draw_ship(const SpaceShip *ship);
void move_ship(SpaceShip *ship, float dt);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");
    SetTargetFPS(60);

    SpaceShip ship;
    init_ship(&ship);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        /* ATUALIZAR */
        move_ship(&ship, dt);

        /* DESENHAR */
        BeginDrawing();
        ClearBackground(BLACK);
        draw_ship(&ship);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void init_ship(SpaceShip *ship) {
    ship->x = 20.0f;
    ship->y = SCREEN_HEIGHT / 2.0f;
    ship->speed = 250.0f;
    ship->lives = 3;
    ship->score = 0;
    ship->limit_x = 6;
    ship->limit_y = 7;
}

void draw_ship(const SpaceShip *ship) {
    float x = ship->x, y = ship->y;
    /* corpo: triângulo apontando para a direita (anti-horário) */
    DrawTriangle((Vector2){x + 12, y}, (Vector2){x - 12, y - 17},
                 (Vector2){x - 12, y + 17}, GREEN);
    /* propulsores */
    DrawRectangle((int)x - 17, (int)y - 9, 5, 6, MAROON);
    DrawRectangle((int)x - 17, (int)y + 3, 5, 6, MAROON);
    /* parte central */
    DrawRectangle((int)x - 12, (int)y - 2, 27, 4, BLUE);
}

void move_ship(SpaceShip *ship, float dt) {
    if (IsKeyDown(KEY_UP))
        ship->y -= ship->speed * dt;
    if (IsKeyDown(KEY_DOWN))
        ship->y += ship->speed * dt;
    if (IsKeyDown(KEY_LEFT))
        ship->x -= ship->speed * dt;
    if (IsKeyDown(KEY_RIGHT))
        ship->x += ship->speed * dt;

    /* limites: só pode andar no primeiro terço da tela */
    if (ship->y < 0)
        ship->y = 0;
    if (ship->y > SCREEN_HEIGHT)
        ship->y = SCREEN_HEIGHT;
    if (ship->x < 0)
        ship->x = 0;
    if (ship->x > SCREEN_WIDTH / 3)
        ship->x = SCREEN_WIDTH / 3;
}
