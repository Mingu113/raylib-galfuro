#ifndef GAMEMAP_H
#define GAMEMAP_H
#include "raylib-cpp.hpp"
#include <fstream>

typedef struct EnviromentObject
{
    EnviromentObject(raylib::Rectangle rect, int blocking, raylib::Color color)
        : rect(rect)
        , blocking(blocking)
        , color(color)
    {}
    raylib::Rectangle rect;
    int blocking;
    raylib::Color color;
} EnviromentObject;
class GameMap
{
public:
    GameMap();
    std::vector<EnviromentObject> map_objects;
    std::vector<std::string> get_map_from(std::string &filename);
    static std::vector<EnviromentObject> get_map(std::vector<std::string> map);
    void draw_map() const;
    void setMap_object(const std::vector<EnviromentObject> &newMap_object);
    void setMap_objects(const std::vector<EnviromentObject> &newMap_objects);
};

#endif // GAMEMAP_H
