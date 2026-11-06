#include "objects.h" /* definições das structs */
#include "raylib.h"
#include "resource_dir.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;

const int NUM_BULLETS = 5;
const int NUM_COMETS = 10;

/* escalas usadas para desenhar as texturas */
const float SHIP_SCALE = 0.5f;
const float COMET_SCALE = 0.4f;
const float BULLET_SCALE = 0.6f;

/* protótipos */
void init_ship(SpaceShip *ship, Texture2D tex);
void draw_ship(const SpaceShip *ship, Texture2D tex);
void move_ship(SpaceShip *ship, float dt);

void init_bullets(Bullet b[], int size);
void fire_bullet(Bullet b[], int size, const SpaceShip *ship);
void draw_bullets(const Bullet b[], int size, Texture2D tex);
void update_bullets(Bullet b[], int size, float dt);

void init_comets(Comet c[], int size, Texture2D tex);
void spawn_comet(Comet c[], int size);
void draw_comets(const Comet c[], int size, Texture2D tex);
void update_comets(Comet c[], int size, float dt);

void colide_bullets(Bullet b[], int bSize, Comet c[], int cSize,
                    SpaceShip *ship);
void colide_comets(Comet c[], int size, SpaceShip *ship);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter");
    SetTargetFPS(60);

    SearchAndSetResourceDir("resources");
    Texture2D ship_tex = LoadTexture("ship.png");
    Texture2D comet_tex = LoadTexture("meteor.png");
    Texture2D bullet_tex = LoadTexture("laser.png");

    SpaceShip ship;
    init_ship(&ship, ship_tex);

    Bullet bullets[NUM_BULLETS];
    init_bullets(bullets, NUM_BULLETS);

    Comet comets[NUM_COMETS];
    init_comets(comets, NUM_COMETS, comet_tex);

    bool game_over = false;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (!game_over) {
            /* ATUALIZAR */
            move_ship(&ship, dt);
            if (IsKeyPressed(KEY_SPACE)) {
                fire_bullet(bullets, NUM_BULLETS, &ship);
            }
            update_bullets(bullets, NUM_BULLETS, dt);
            spawn_comet(comets, NUM_COMETS);
            update_comets(comets, NUM_COMETS, dt);
            colide_bullets(bullets, NUM_BULLETS, comets, NUM_COMETS, &ship);
            colide_comets(comets, NUM_COMETS, &ship);
            if (ship.lives <= 0)
                game_over = true;
        } else if (IsKeyPressed(KEY_ENTER)) { /* reiniciar */
            init_ship(&ship, ship_tex);
            init_bullets(bullets, NUM_BULLETS);
            init_comets(comets, NUM_COMETS, comet_tex);
            game_over = false;
        }

        /* DESENHAR */
        BeginDrawing();
        ClearBackground(BLACK);
        if (!game_over) {
            draw_ship(&ship, ship_tex);
            draw_bullets(bullets, NUM_BULLETS, bullet_tex);
            draw_comets(comets, NUM_COMETS, comet_tex);
            DrawText(
                TextFormat("Lives: %d   Score: %d", ship.lives, ship.score), 5,
                5, 20, WHITE);
        } else {
            const char *msg = TextFormat("End of game. Score: %d", ship.score);
            int txt_width = MeasureText(msg, 30);
            DrawText(msg, (SCREEN_WIDTH - txt_width) / 2,
                     SCREEN_HEIGHT / 2 - 30, 30, WHITE);
            DrawText("ENTER to restart",
                     (SCREEN_WIDTH - MeasureText("ENTER to restart", 20)) / 2,
                     SCREEN_HEIGHT / 2 + 10, 20, GRAY);
        }
        EndDrawing();
    }

    UnloadTexture(ship_tex);
    UnloadTexture(comet_tex);
    UnloadTexture(bullet_tex);
    CloseWindow();
    return 0;
}

void init_ship(SpaceShip *ship, Texture2D tex) {
    ship->x = 40.0f;
    ship->y = SCREEN_HEIGHT / 2.0f;
    ship->speed = 250.0f;
    ship->lives = 3;
    ship->score = 0;
    /* a imagem aponta para cima e será girada 90 graus:
       largura e altura trocam de lugar na tela */
    ship->limit_x = (int)(tex.height * SHIP_SCALE / 2);
    ship->limit_y = (int)(tex.width * SHIP_SCALE / 2);
}

void draw_ship(const SpaceShip *ship, Texture2D tex) {
    Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
    Rectangle dst = {ship->x, ship->y, tex.width * SHIP_SCALE,
                     tex.height * SHIP_SCALE};
    Vector2 origin = {dst.width / 2, dst.height / 2}; /* centro */
    DrawTexturePro(tex, src, dst, origin, 90.0f, WHITE);
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

    /* limites: a nave inteira fica dentro da tela */
    if (ship->y < ship->limit_y)
        ship->y = ship->limit_y;
    if (ship->y > SCREEN_HEIGHT - ship->limit_y)
        ship->y = SCREEN_HEIGHT - ship->limit_y;
    if (ship->x < ship->limit_x)
        ship->x = ship->limit_x;
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
        if (!b[i].active) {
            b[i].x = ship->x + ship->limit_x;
            b[i].y = ship->y;
            b[i].active = true;
            break;
        }
    }
}

void draw_bullets(const Bullet b[], int size, Texture2D tex) {
    Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
    for (int i = 0; i < size; i++) {
        if (b[i].active) {
            Rectangle dst = {b[i].x, b[i].y, tex.width * BULLET_SCALE,
                             tex.height * BULLET_SCALE};
            Vector2 origin = {dst.width / 2, dst.height / 2};
            DrawTexturePro(tex, src, dst, origin, 90.0f, WHITE);
        }
    }
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

void init_comets(Comet c[], int size, Texture2D tex) {
    for (int i = 0; i < size; i++) {
        c[i].active = false;
        c[i].speed = 300.0f;
        c[i].limit_x = (int)(tex.width * COMET_SCALE / 2);
        c[i].limit_y = (int)(tex.height * COMET_SCALE / 2);
    }
}

void spawn_comet(Comet c[], int size) {
    for (int i = 0; i < size; i++) {
        if (!c[i].active) {
            if (GetRandomValue(0, 499) == 0) {
                c[i].active = true;
                c[i].x = SCREEN_WIDTH + c[i].limit_x; /* nasce fora da tela */
                c[i].y = (float)GetRandomValue(30, SCREEN_HEIGHT - 30);
                break;
            }
        }
    }
}

void draw_comets(const Comet c[], int size, Texture2D tex) {
    for (int i = 0; i < size; i++) {
        if (c[i].active) {
            /* DrawTextureEx usa o canto superior esquerdo: descontar os limites
             */
            Vector2 pos = {c[i].x - c[i].limit_x, c[i].y - c[i].limit_y};
            DrawTextureEx(tex, pos, 0.0f, COMET_SCALE, WHITE);
        }
    }
}

void update_comets(Comet c[], int size, float dt) {
    for (int i = 0; i < size; i++) {
        if (c[i].active) {
            c[i].x -= c[i].speed * dt;
        }
    }
}

void colide_bullets(Bullet b[], int bSize, Comet c[], int cSize,
                    SpaceShip *ship) {
    for (int i = 0; i < bSize; i++) {
        if (!b[i].active)
            continue;
        for (int j = 0; j < cSize; j++) {
            if (!c[j].active)
                continue;
            if (b[i].x > c[j].x - c[j].limit_x &&
                b[i].x < c[j].x + c[j].limit_x &&
                b[i].y > c[j].y - c[j].limit_y &&
                b[i].y < c[j].y + c[j].limit_y) {
                b[i].active = false;
                c[j].active = false;
                ship->score++;
            }
        }
    }
}

void colide_comets(Comet c[], int size, SpaceShip *ship) {
    for (int i = 0; i < size; i++) {
        if (!c[i].active)
            continue;
        if (c[i].x - c[i].limit_x < ship->x + ship->limit_x &&
            c[i].x + c[i].limit_x > ship->x - ship->limit_x &&
            c[i].y - c[i].limit_y < ship->y + ship->limit_y &&
            c[i].y + c[i].limit_y > ship->y - ship->limit_y) {
            ship->lives--;
            c[i].active = false;
        } else if (c[i].x < 0) { /* cometa passou pela nave */
            c[i].active = false;
            ship->lives--;
        }
    }
}
