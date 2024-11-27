#ifndef ENEMIES_H
#define ENEMIES_H
#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f
#define ENEMY_SPD 200.f

#include "raylib-cpp.hpp"
#include "gamemap.h"
class Player;
class Bullet;
enum look_at {right = 1, left = -1};
enum action {moving, attacking, waiting};
class Enemies
{
public:
    raylib::Vector2 position;
    raylib::Rectangle detection; // width have to be long, a horonziteniontal whatever rectangle
    look_at direction;
    raylib::Rectangle rect;
    float speed;
    float health;
    bool isAlive;
    //
    bool isHunting = false;
    double hunting_start = 0;
    double death_time = 0;
    double waiting = 0;
    raylib::Vector2 last_pos;
    bool spotted = false;
    look_at last_direction;
    raylib::Vector2 last_seen_position = {0};
    //
    Enemies();
    Enemies(raylib::Vector2 position, float health, float detection_range)
        : position(position)
        , health(health)
    {
        isAlive = true;
        last_direction = direction = left;
        speed = 1;
        last_pos = position;
        rect = raylib::Rectangle(position, raylib::Vector2(50, 50));
        this->detection = (raylib::Rectangle) {(raylib::Vector2){position.x - detection_range + rect.width, position.y + rect.height}, (raylib::Vector2){detection_range, rect.GetHeight() + 10}};
    }
    void update(Player *player, std::vector<EnviromentObject> *enobj, float delta);
    void move_to(raylib::Vector2 &destination, float delta);
    void move_to(Player &player, float delta);
    void draw() const;
    void got_hit(Bullet bullet, float damage);
};

#endif // ENEMIES_H