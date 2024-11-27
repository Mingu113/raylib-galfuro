#include "gamemap.h"
#include <vector>
GameMap::GameMap()
{
    std::string file("map.txt");
    map_objects = get_map(get_map_from(file));
}
std::vector<std::string> GameMap::get_map_from(std::string &path_to_file)
{
    std::vector<std::string> map(1, "");
    std::ifstream input(path_to_file);
    if (!input) {
        return map;
    }
    if (input.is_open()) {
        map.clear();
        for (std::string line; std::getline(input, line);) {
            map.emplace_back(line);
        }
    }
    return map;
}
std::vector<EnviromentObject> GameMap::get_map(std::vector<std::string> map)
{
    std::vector<EnviromentObject> rectangles;
    std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map[0].size(), false));
    for (size_t var = 0; var < map.size(); ++var) {
        std::string current_line = map.at(var);
        for (size_t i = 0; i < current_line.size(); ++i) {
            if (current_line[i] == '*' && visited[var][i] == false) {
                int y = var;
                int x = i;
                int width = 0;
                while (current_line[i] == '*' && i < current_line.size()) {
                    width++;
                    visited[var][i++] = true;
                }
                size_t temp = y;
                int height = 0;
                while (map[temp][x + width - 1] == '*' && temp < map.size() - 1) {
                    height++;
                    int a = x;
                    while (a < x + width) {
                        visited[temp][a++] = true;
                    }
                    temp++;
                }
                int size = 50;
                rectangles.emplace_back(EnviromentObject(
                    raylib::Rectangle(x * size, y * size, width * size, height * size), 1, GRAY));
            }
        }
    }
    return rectangles;
}
void GameMap::draw_map() const
{
    for (const auto &ei : map_objects) {
        ei.rect.Draw(ei.color);
    }
}
