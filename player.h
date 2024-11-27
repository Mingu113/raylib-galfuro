#ifndef PLAYER_H
#define PLAYER_H
#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f
#define MAX_BULLET_TRAVEL 500.0f;
#include "raylib-cpp.hpp"
#include "gamemap.h"
#include "enemies.h"
#include <limits>
class Bullet
{
public:

    float speed;
    raylib::Vector2 position;
    double stay = 0.1;
    double hit_time = 0;
    bool still = true;
    double dest = std::numeric_limits<double>::quiet_NaN();
    Bullet(raylib::Vector2 position, float speed) : speed(speed), position(position) {

    }
    void shot();
    void draw() {
        if(still) {
            if(speed > 0)
            {
                if(std::isnan(dest)) dest = std::numeric_limits<int>::max();
            }
            else
            {
                if(std::isnan(dest)) dest = std::numeric_limits<int>::min();
            }
            DrawLine(position.x, position.y, dest, position.y, BLACK);
        }
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
    action player_is;
    Player(raylib::Vector2 position, float speed, bool canJump, raylib::Color color)
        : position(position)
        , speed(speed)
        , canJump(canJump)
        , color(color)
    {
        direction = right;
        player_is = waiting;
    }
    void draw_player();
    void draw_bullets();
    void update_bullets(const std::vector<EnviromentObject> *eni, std::vector<Enemies> *enemies);
    static void updatePlayer(Player *player, std::vector<EnviromentObject> *envObjs, std::vector<Enemies> *enemies, float delta);
    static void updateCamera(raylib::Camera2D *camera, const Player *player, int width, int height);

};

#endif // PLAYER_H
