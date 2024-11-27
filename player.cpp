#include "player.h"
#include "raylib-cpp.hpp"
#include <any>
#include <limits>
#include <string>
#include <typeinfo>
void Player::draw_bullets()
{
    for(auto& bullet : bullets) {
        bullet.draw();
    }
}
//  Update bullets and remove them out of the game in runtime so the game won't have too many of them, or check too many of them in every game loop
//  If the bullet hit something, it will interact with the object that it is hit, and got removed out of the bullets vector <-- Old methos
//  If the bullet line hit something, it will disapear
void Player::update_bullets(const std::vector<EnviromentObject> *eni, std::vector<Enemies> *enemies) {
    for (size_t index = 0; index < bullets.size(); ++index) {
        std::any nearest;
        int nearest_index = -1;
        int shortest_distance = std::numeric_limits<int>::max();
        double dest;
        auto& bullet = bullets.at(index);
        if(bullet.hit_time != 0 && GetTime() - bullet.stay >= bullet.hit_time)
        {
            bullets.erase(bullets.begin() + index);
            break;
        }
        for (int eo_i = 0; eo_i < eni->size(); ++eo_i) {
            auto& ei = eni->at(eo_i);
            // Check for stuff that bullet might hit
            ///                 |---|
            ///      bullet     |   |
            ///         ------> |   |
            ///                 |---|
            if(bullet.position.y >= ei.rect.y && bullet.position.y <= ei.rect.y + ei.rect.height) {
                // If that stuff is on the right
                if(bullet.speed > 0 && ei.rect.x > bullet.position.x) {
                    if(ei.rect.x - bullet.position.x < shortest_distance) {
                        nearest = &ei;
                        shortest_distance = ei.rect.x - bullet.position.x;
                        nearest_index = eo_i;
                        dest = ei.rect.x;
                    }
                    // Or on the left
                } else if (bullet.speed < 0 && ei.rect.x + ei.rect.width < bullet.position.x) {
                    if(bullet.position.x - (ei.rect.x + ei.rect.width) < shortest_distance) {
                        nearest = &ei;
                        shortest_distance = bullet.position.x - ei.rect.x;
                        nearest_index = eo_i;
                        dest = ei.rect.x + ei.rect.width;
                    }
                }
            }
        }
        // Same logic for enemies
        for (int e_i = 0; e_i < enemies->size(); ++e_i) {
            auto& enemy = enemies->at(e_i);
            if(bullet.position.y >= enemy.rect.y && bullet.position.y <= enemy.rect.y + enemy.rect.height) {
                if(bullet.speed > 0 && enemy.rect.x > bullet.position.x) {
                    if(enemy.rect.x - bullet.position.x < shortest_distance) {
                        nearest = &enemy;
                        nearest_index = e_i;
                        shortest_distance = enemy.rect.x - bullet.position.x;
                        dest = enemy.rect.x;
                    }
                } else if (bullet.speed < 0 && enemy.rect.x + enemy.rect.width < bullet.position.x) {
                    if(bullet.position.x - (enemy.rect.x + enemy.rect.width) < shortest_distance) {
                        nearest = &enemy;
                        nearest_index = e_i;
                        shortest_distance = bullet.position.x - enemy.rect.x;
                        dest = enemy.rect.x + enemy.rect.width;
                    }
                }
            }
        }
        // Used std::any, while both give Enemies and EnviromentObject, the nearest give different front symbol so
        // the equation will likely never be true
        // Use contains
        // It doesn't have contains, use find instead
        if (shortest_distance != std::numeric_limits<int>::max()) {
            if (nearest_index != -1) {
                const std::string str(nearest.type().name());
                bullet.dest = dest;
                if (str.find("Enviroment") != std::string::npos) {
                } else if (str.find("Enemies") != std::string::npos) {
                    enemies->at(nearest_index).got_hit(bullets.at(index), 25);
                }
            }
        }
        bullet.shot();
        // Under is old method
        /*
        bullet.update();
        for(const auto& ei : *eni) {
            if(bullet.position.CheckCollision(ei.rect)) {
                bullets.erase(bullets.begin() + index);
                break; // so it is already hit something, not more loop or check if it hit enemy. that thing is not gonna ricochet
            }
        }
        // I wanted to mark if the bullet have already hit something and skip checking, but if I do that, the bullet go through the enemy
        for(auto& enemy : *enemies) {
            if(bullet.position.CheckCollision(enemy.rect)){
                enemy.got_hit(bullet, 5);
                bullets.erase(bullets.begin() + index);
                break; // same with this
            }
        }
        // if it does not hit anything, it will fly forever until the program end. or MAX_FLOAT
        if(bullet.position.x > position.x + 1000 + 500 || bullet.position.x < position.x - 1000 - 500) { // Ideally, player pos + screen width + a max distance, this is hardcoded and is not good
            bullets.erase(bullets.begin() + index); // remove that bullet
        }
        // I may check for y too, but it will eventually go above player, so no need
    */
    }
}
// Update player movement, action and it objects (bullets)
void Player::updatePlayer(Player *player, std::vector<EnviromentObject> *envObjs, std::vector<Enemies> *enemies, float delta)
{
    // This is for cool down so the player can't shoot 1 bullet per frame, turning it into laser, and lag the game. and also unrealisitc
    double cool_down = 0.2;
    static bool can_shoot = true;
    static double last_shot;
    player->player_is = waiting;
    if(GetTime() - last_shot >= cool_down) {
        can_shoot = true;
    }
    if (raylib::Keyboard::IsKeyDown(KEY_RIGHT))
    {
        player->player_is = moving;
        player->position.x += PLAYER_HOR_SPD * delta;
        player->direction = right;
    }
    if (raylib::Keyboard::IsKeyDown(KEY_LEFT))
    {
        player->player_is = moving;
        player->position.x -= PLAYER_HOR_SPD * delta;
        player->direction = left;
    }
    if (raylib::Keyboard::IsKeyDown(KEY_SPACE) && player->canJump) {
        player->speed -= PLAYER_JUMP_SPD;
        player->canJump = false;
    }
    if(raylib::Keyboard::IsKeyDown(KEY_Z) && can_shoot) {
        player->player_is = attacking;
        float bullet_speed = 70;
        player->bullets.emplace_back(Bullet((raylib::Vector2){player->position.GetX(), player->position.GetY() - 20}, player->direction == right ? bullet_speed : -bullet_speed));
        can_shoot = false;
        last_shot = GetTime();
    }
    bool is_on_object = false;
    for (const auto &ei : *envObjs) {
        Vector2 *p = &(player->position);
        if (ei.blocking){
            if(ei.rect.x <= p->x &&
            ei.rect.x + ei.rect.width >= p->x &&
            ei.rect.y >= p->y)
            {
            if(ei.rect.y <= p->y + player->speed * delta)
                {
                is_on_object = true;
                player->speed = 0.0f;
                p->y = ei.rect.y;
                }
            }
            if(player->position.CheckCollision(ei.rect) && p->y > ei.rect.y) {
                if(p->x > ei.rect.x && p->x < ei.rect.x + 10) {
                    p->x = ei.rect.x;
                } else
                if(p->x < ei.rect.x + ei.rect.width && p->x > ei.rect.x + ei.rect.width - 10) {
                    p->x = ei.rect.x + ei.rect.width;
                } else
                if(p->y <= ei.rect.y + ei.rect.height) {
                    player->speed = 0.0f;
                    p->y = ei.rect.y + ei.rect.height;
                }
            }
        }
    }
    if (!is_on_object) {
        player->position.y += player->speed * delta;
        player->speed += G * delta;
        player->canJump = false;
    } else
        player->canJump = true;
    player->update_bullets(envObjs, enemies);
}
// update the player camera
void Player::updateCamera(raylib::Camera2D *camera, const Player *player, int width, int height)
{
    static raylib::Vector2 bbox(0.2f, 0.05f);
    raylib::Vector2 bboxWorldMin = camera->GetScreenToWorld(
        (raylib::Vector2){(1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * width});
    raylib::Vector2 bboxWorldMax = camera->GetScreenToWorld(
        (raylib::Vector2){(1 + bbox.x) * 0.5f * width, (1 + bbox.y) * 0.5f * width});

    camera->offset = (raylib::Vector2){(1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height};

    if (player->position.x < bboxWorldMin.x)
        camera->target.x = player->position.x;
    if (player->position.y < bboxWorldMin.y)
        camera->target.y = player->position.y;
    if (player->position.x > bboxWorldMax.x)
        camera->target.x = bboxWorldMin.x + (player->position.x - bboxWorldMax.x);
    if (player->position.y > bboxWorldMax.y)
        camera->target.y = bboxWorldMin.y + (player->position.y - bboxWorldMax.y);
}
//  Draw player and bullets that it have shot
void Player::draw_player() {
    float ballRadius = 20;
    this->draw_bullets();
    DrawCircleV((raylib::Vector2){this->position.x, this->position.y - ballRadius},
                ballRadius,
                this->color);

}
void Bullet::shot() {
    if (hit_time == 0)
        hit_time = GetTime();
}