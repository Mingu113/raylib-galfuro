#ifndef ANIMATION_H
#define ANIMATION_H
#include "raylib-cpp.hpp"
class animation
{
public:
    raylib::Texture2D source;
    raylib::Rectangle frameRec;
    animation(std::string source, int number_of_frame);
    void update();
private:
    int number_of_frames;
    int frames_speed = 25;
    int current_frame = 0;
    int frame_counter = 0;
};

#endif // ANIMATION_H
