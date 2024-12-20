#ifndef PLAYER_H
#define PLAYER_H
#include <map>
#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f
#define MAX_BULLET_TRAVEL 500.0f;
#include "raylib-cpp.hpp"
#include "gamemap.h"
#include "enemies.h"
#include "animation.h"
#include <limits>
class Bullet
{
public:

    float speed;
    raylib::Vector2 position;
    double stay = 0.1;
    double hit_time = 0;
    bool still = true;
    bool do_damage = true;
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
    raylib::Rectangle size;
    std::map<action, animation> player_anim;
    Player(raylib::Vector2 position, float speed, bool canJump, raylib::Color color, std::map<action, animation> animation)
        : position(position)
        , speed(speed)
        , canJump(canJump)
        , color(color)
    {
        player_anim = animation;
        direction = right;
        player_is = waiting;
        size = player_anim[action::waiting].frameRec;
        size.SetPosition(position);
    }
    void draw_player();
    void draw_bullets();
    void update_bullets(const std::vector<EnviromentObject> *eni, std::vector<Enemies> *enemies);
    void updatePlayer(std::vector<EnviromentObject> *envObjs, std::vector<Enemies> *enemies, float delta);
    void updateCamera(raylib::Camera2D *camera, int width, int height);

};

#endif // PLAYER_H
