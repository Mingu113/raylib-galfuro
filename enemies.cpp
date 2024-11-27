#include "enemies.h"
#include "player.h"

//  Even though the enemy behaviour should behave per enemies, but somehow they act all the same time.
// It seem that even though the static varibles is created at run time, it is used across the class?
// Whatever it is, it broke some of the features that took a long time to implement

// move enemy to the destination, use delta for speed and stuff
void Enemies::move_to(raylib::Vector2 &destination, float delta)
{
    if (this->position.x < destination.x) {
        direction = right;
        this->position.x += ENEMY_SPD * delta;
        // if (this->position.x > destination.x)
        //     this->position.x = destination.x;
        this->position.x = std::max(this->position.x, destination.x);
    } else if (this->position.x > destination.x) {
        direction = left;
        this->position.x -= ENEMY_SPD * delta;
        // if (this->position.x < destination.x)
        //     this->position.x = destination.x;
        this->position.x = std::min(this->position.x, destination.x);
    }
}
void Enemies::move_to(Player &player, float delta)
{
    // don't go to player's x, sekuhara is another type of game
    float distance = 100;
    if (this->position.x < player.position.x - distance) {
        direction = right;
        this->position.x += ENEMY_SPD * delta;
        if (this->position.x > player.position.x + distance)
            this->position.x = player.position.x;
    } else if (this->position.x > player.position.x + distance) {
        direction = left;
        this->position.x -= ENEMY_SPD * delta;
        if (this->position.x < player.position.x - distance)
            this->position.x = player.position.x;
    }
}
// update enemies per frame
void Enemies::update(Player *player, std::vector<EnviromentObject> *enobj, float delta)
{
    rect.SetPosition(position);
    // check enemy health
    if (this->health <= 0 && isAlive) {
        // wait for a while for the enemy before disapear
        double cool_down = 3.0;
        if (GetTime() - cool_down >= this->death_time) {
            this->isAlive = false;
        }
    }
    // physics logic
    bool hitObstacle = false;
    for (const auto &ei : *enobj) {
        if (ei.blocking && ei.rect.x <= position.x && ei.rect.x + ei.rect.width >= position.x
            && ei.rect.y >= position.y - rect.height
            && ei.rect.y <= position.y + rect.height
                                + speed * delta) /*This code works, reason unknown*/ {
            hitObstacle = true;
            speed = 0.0f;
            position.y = ei.rect.y - rect.GetHeight();
            break;
        }
    }
    if (!hitObstacle) {
        position.y += delta * speed;
        speed += G * delta;
    }
    // detection logic
    if (this->health > 0) {
        detection.x = direction == left ? rect.x - detection.width + rect.width : rect.x;
        detection.y = rect.y - 5;
        double cool_down = 2;
        double hunt_cool_down = 3;

        if (player->position.CheckCollision(detection)) {
            spotted = true;
            last_pos = rect.GetPosition();
            last_direction = direction;
            last_seen_position = player->position;
            isHunting = true;
            hunting_start = GetTime();
        }
        if (spotted || isHunting) {
            // In hunt mode
            if (isHunting) {
                // hunt mode expired
                if (GetTime() - hunt_cool_down >= hunting_start)
                    isHunting = false;
                // else
                move_to(*player, delta);
            } else
                move_to(last_seen_position, delta);
            if (this->position.x == last_seen_position.x) {
                spotted = false;
                waiting = GetTime();
            }
        }
        if (!spotted && !isHunting) {
            // cool down, go back to last position
            if (GetTime() - cool_down >= waiting) {
                move_to(last_pos, delta);
                if (this->position.x == last_pos.x) {
                    this->direction = last_direction;
                }
            }
        }
    }
}

void Enemies::draw() const
{
    detection.DrawLines(PINK);
    raylib::Color health = this->health > 0 ? raylib::Color::Green() : raylib::Color::Red();
    rect.Draw(health);
}

void Enemies::got_hit(Bullet bullet, float damage)
{
    // bullet is from the left
    if (bullet.position.x < this->position.x) {
        direction = left;
    } else
        direction = right;
    this->health -= damage;
    this->isHunting = true;
    this->hunting_start = GetTime();
    if (this->health <= 0) {
        if (this->death_time == 0)
            this->death_time = GetTime();
        this->health = 0;
    }
}
