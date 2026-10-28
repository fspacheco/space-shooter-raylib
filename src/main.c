#include "objects.h" /* definições das structs */
#include "raylib.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;

const int NUM_BULLETS = 5;

const int NUM_COMETS = 10;

/* protótipos */
void init_ship(SpaceShip *ship);
void draw_ship(const SpaceShip *ship);
void move_ship(SpaceShip *ship, float dt);

void init_bullets(Bullet b[], int size);
void fire_bullet(Bullet b[], int size, const SpaceShip *ship);
void draw_bullets(const Bullet b[], int size);
void update_bullets(Bullet b[], int size, float dt);

void init_comets(Comet c[], int size);
void spawn_comet(Comet c[], int size);
void draw_comets(const Comet c[], int size);
void update_comets(Comet c[], int size, float dt);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");
    SetTargetFPS(60);

    SpaceShip ship;
    init_ship(&ship);

    Bullet bullets[NUM_BULLETS];
    init_bullets(bullets, NUM_BULLETS);

    Comet comets[NUM_COMETS];
    init_comets(comets, NUM_COMETS);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        /* ATUALIZAR */
        move_ship(&ship, dt);
        if (IsKeyPressed(KEY_SPACE)) {
            fire_bullet(bullets, NUM_BULLETS, &ship);
        }
        update_bullets(bullets, NUM_BULLETS, dt);
        spawn_comet(comets, NUM_COMETS);
        update_comets(comets, NUM_COMETS, dt);

        /* DESENHAR */
        BeginDrawing();
        ClearBackground(BLACK);
        draw_ship(&ship);
        draw_bullets(bullets, NUM_BULLETS);
        draw_comets(comets, NUM_COMETS);
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

void init_bullets(Bullet b[], int size) {
    for (int i = 0; i < size; i++) {
        b[i].active = false;
        b[i].speed = 600.0f;
    }
}

void fire_bullet(Bullet b[], int size, const SpaceShip *ship) {
    for (int i = 0; i < size; i++) {
        if (!b[i].active) { /* primeiro tiro livre */
            b[i].x = ship->x + 17;
            b[i].y = ship->y;
            b[i].active = true;
            break; /* só um tiro por disparo */
        }
    }
}

void draw_bullets(const Bullet b[], int size) {
    for (int i = 0; i < size; i++)
        if (b[i].active)
            DrawCircle((int)b[i].x, (int)b[i].y, 2.0f, WHITE);
}

void update_bullets(Bullet b[], int size, float dt) {
    for (int i = 0; i < size; i++) {
        if (b[i].active) {
            b[i].x += b[i].speed * dt;
            if (b[i].x > SCREEN_WIDTH)
                b[i].active = false; /* saiu da tela */
        }
    }
}

void init_comets(Comet c[], int size) {
    for (int i = 0; i < size; i++) {
        c[i].active = false;
        c[i].speed = 300.0f;
        c[i].limit_x = 18;
        c[i].limit_y = 18;
    }
}

void spawn_comet(Comet c[], int size) {
    for (int i = 0; i < size; i++) {
        if (!c[i].active) {
            if (GetRandomValue(0, 499) == 0) { /* chance 1/500 por frame */
                c[i].active = true;
                c[i].x = SCREEN_WIDTH;
                c[i].y = (float)GetRandomValue(30, SCREEN_HEIGHT - 30);
                break;
            }
        }
    }
}

void draw_comets(const Comet c[], int size) {
    for (int i = 0; i < size; i++)
        if (c[i].active)
            DrawCircle((int)c[i].x, (int)c[i].y, 20.0f, RED);
}

void update_comets(Comet c[], int size, float dt) {
    for (int i = 0; i < size; i++) {
        if (c[i].active) {
            c[i].x -= c[i].speed * dt;
            if (c[i].x < 0)
                c[i].active = false;
        }
    }
}
