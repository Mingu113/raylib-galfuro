#include "enemies.h"
#include "player.h"
#include <iostream>

//  Even though the enemy behaviour should behave per enemies, but somehow they act all the same time.
// It seem that even though the static varibles is created at run time, it is used across the class?
// Whatever it is, it broke some of the features that took a long time to implement

// It took a lots of time to correct most of the things I spent a lots of time on implement it.
/// Sound like it pay off, since it look cleaner.
/// a bit

// move enemy to the destination, use delta for speed and stuff
void Enemies::move_to(raylib::Vector2 &destination, float delta)
{
    if (this->position.x < destination.x) {
        direction = right;
        this->position.x += ENEMY_SPD * delta;
        if (this->position.x > destination.x)
            this->position.x = destination.x;
    } else if (this->position.x > destination.x) {
        direction = left;
        this->position.x -= ENEMY_SPD * delta;
        if (this->position.x < destination.x)
            this->position.x = destination.x;
    }
}
void Enemies::move_to(Player &player, float delta)
{
    // don't go to player's x, sekuhara is another type of game
    /// Fix later
    float distance = 200;
    if (this->position.x < player.position.x + distance) {
        direction = right;
        this->position.x += ENEMY_SPD * delta;
        if (this->position.x > player.position.x + distance)
            this->position.x = player.position.x + distance;
    } else if (this->position.x > player.position.x - distance) {
        direction = left;
        this->position.x -= ENEMY_SPD * delta;
        if (this->position.x < player.position.x - distance)
            this->position.x = player.position.x - distance;
    }
}
// update enemies per frame
/// Call GetTime() too many. might as well assign it to a variable to reduce the number of time call function
void Enemies::update(Player *player, std::vector<EnviromentObject> *enobj, float delta)
{
    static raylib::Vector2 last_checked_pos;
    static double check_time = 0; // second
    // check enemy health
    if (this->health <= 0 && isAlive) {
        // wait for a while for the enemy before disapear
        double cool_down = 3.0;
        if (GetTime() - cool_down >= this->death_time) {
            this->isAlive = false;
        }
    }

    // Taken from Player physic logic
    bool is_on_object = false;
    for (const auto &ei : *enobj) {
        Vector2 p = position;
        if (ei.blocking){
            // Above
            // use size.checkCollision some how doesn't work, and I do not know why
            if(ei.rect.x <= rect.x + rect.width &&
                ei.rect.x + ei.rect.width >= rect.x &&
                ei.rect.y >= rect.y + rect.height)
            {
                if(ei.rect.y <= rect.y + rect.height + speed * delta)
                {
                    is_on_object = true;
                    speed = 0.0f;
                    position.y = ei.rect.y;
                }
            }
            // Sides and bellow
            else
            if(rect.CheckCollision(ei.rect) && rect.y + rect.height > ei.rect.y) {
                // Left
                if(rect.x + rect.width >= ei.rect.x && rect.x + rect.width < ei.rect.x + 5) {
                    position.x = ei.rect.x - rect.width / 2;
                } else
                // Right
                if(rect.x <= ei.rect.x + ei.rect.width && rect.x > ei.rect.x + ei.rect.width - 5) {
                    position.x = ei.rect.x + ei.rect.width + rect.width / 2;
                } else
                // Bellow
                if(rect.y <= ei.rect.y + ei.rect.height) {
                    speed = 0.0f;
                    position.y = ei.rect.y + ei.rect.height + rect.height;
                }
            }
        }
    }
    if (!is_on_object) {
        position.y += speed * delta;
        speed += G * delta;
        canJump = false;
    } else
        canJump = true;
    rect.SetPosition({position.x - rect.width / 2, position.y - rect.height});
    // detection logic
    if (this->health > 0) {
        detection.x = direction == left ? rect.x - detection.width + rect.width : rect.x;
        detection.y = rect.y - 5;
        double cool_down = 2;
        double hunt_cool_down = 3;

        if (player->size.CheckCollision(detection)) {
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
            /// Jumping logic, if enemies is stuck, try jumping
            /// Check for last position, see if this is stuck
            if(GetTime() - check_time >= cool_down)
            {
                last_checked_pos = rect.GetPosition();
                check_time = GetTime();
            }
            // stand in one place
            if(GetTime() - last_jump >= jump_cool_down && canJump && rect.CheckCollision(last_checked_pos))
            {
                std::cout << "Jumped\n";
                speed -= PLAYER_JUMP_SPD;
                canJump = false;
                last_jump = GetTime();
            }
            /// Shouldn't I put this in the move_to function? since it make more sense
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
    position.DrawCircle(10, RED);
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
