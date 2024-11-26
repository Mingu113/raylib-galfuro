#ifndef PLAYER_H
#define PLAYER_H
#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f
#define MAX_BULLET_TRAVEL 500.0f;
#include "raylib-cpp.hpp"
#include "gamemap.h"
#include "enemies.h"
class Bullet
{
public:

    float speed;
    raylib::Vector2 position;
    Bullet(raylib::Vector2 position, float speed) : speed(speed), position(position) {

    }
     void draw() const {
        DrawCircleV(position, 5, raylib::Color::Blue());
    }
    void update() {
        position.x += speed;
    }
};
class Player
{
public:
    raylib::Vector2 position;
    float speed;
    bool canJump;
    look_at direction = right;
    raylib::Color color;
    std::vector<Bullet> bullets;
    Player(raylib::Vector2 position, float speed, bool canJump, raylib::Color color)
        : position(position)
        , speed(speed)
        , canJump(canJump)
        , color(color)
    {
        direction = right;
    }
    void draw_player()const;
    void draw_bullets() const;
    void update_bullets(const std::vector<EnviromentObject> *eni, std::vector<Enemies> *enemies);
    static void updatePlayer(Player *player, std::vector<EnviromentObject> *envObjs, std::vector<Enemies> *enemies, float delta);
    static void updateCamera(raylib::Camera2D *camera, const Player *player, int width, int height);

};

#endif // PLAYER_H
