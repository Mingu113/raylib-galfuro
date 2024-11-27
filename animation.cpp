#include "animation.h"

animation::animation(std::string source, int number_of_frame)
{
    this->source.Load("source");
    this->frameRec = raylib::Rectangle(0, 0, this->source.width, this->source.height);
}

void animation::update()
{
    this->frame_counter++;
    if(frame_counter >= 60 / frames_speed)
    {
        frame_counter = 0;
        current_frame++;

        if(current_frame > number_of_frames) current_frame = 0;
        frameRec.x = current_frame * (source.width / number_of_frames);
    }
}
