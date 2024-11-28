#include "animation.h"

bool animation::operator==(const animation &other) const
{
    return source.GetId() == other.source.GetId();
}

animation::animation(std::string source, int number_of_frame)
{
    this->source.Load(source);
    this->number_of_frames = number_of_frame;
    this->frameRec = raylib::Rectangle(0, 0, this->source.width / number_of_frame, this->source.height);
}



void animation::draw(raylib::Vector2 pos, look_at direction)
{
    if(direction == left)
    {
        raylib::Rectangle rec = frameRec;
        rec.width *= -1;
        source.Draw(rec, pos, WHITE);
    }
    else source.Draw(frameRec, pos, WHITE);
}
