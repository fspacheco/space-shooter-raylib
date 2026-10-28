#ifndef OBJECTS_H
#define OBJECTS_H

typedef struct {
    float x, y;
    float speed; /* px/s */
    int lives;
    int score;
    int limit_x; /* metade da largura (colisão) */
    int limit_y; /* metade da altura  (colisão) */
} SpaceShip;

typedef struct {
    float x, y;
    bool active; /* o tiro está em voo? */
    float speed;
} Bullet;

typedef struct {
    float x, y;
    bool active;
    float speed;
    int limit_x;
    int limit_y;
} Comet;

#endif