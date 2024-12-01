#ifndef ANIMATION_H
#define ANIMATION_H
#include "raylib-cpp.hpp"
enum look_at {right = 1, left = -1};
enum action {moving, attacking, waiting, shoot};
class animation
{
public:
    raylib::Texture2D source;
    raylib::Rectangle frameRec;
    bool operator==(const animation& other) const;
    bool operator<(const animation& other) const
    {
        return this->source.id < other.source.id;
    }
    animation()
    {
        this->source.Load("example.png");
    }
    animation(const animation& other)
    {
        this->source = LoadTextureFromImage(other.source.GetData());
        number_of_frames = other.number_of_frames;
        frameRec = other.frameRec;
    }
    animation& operator=(const animation& other)
    {
        this->source = LoadTextureFromImage(other.source.GetData());
        this->frameRec = other.frameRec;
        this->number_of_frames = other.number_of_frames;
        return *this;
    }
    animation(std::string source, int number_of_frame);
    /// I tried to resize the image sources, while it does resized and is smaller, the animtion went horribly wrong
    /// and I do not know why
    void update()
    {
        this->frame_counter++;
        if(frame_counter >= 60 / frames_speed)
        {
            frame_counter = 0;
            current_frame++;

            if(current_frame > number_of_frames - 1) current_frame = 0;
            frameRec.x = (float)current_frame * (float)(source.width / number_of_frames);
        }
    }
    void draw(raylib::Vector2 pos, look_at direction);
    void reset_frame()
    {
        current_frame = 0;
    }
    void freeze()
    {
        current_frame = this->current_frame - frame_counter;
    }
private:
    int number_of_frames = 1;
    int frames_speed = 25;
    int current_frame = 0;
    int frame_counter = 0;
};

#endif // ANIMATION_H
