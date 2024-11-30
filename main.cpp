// #include "raylib.h"
#include "enemies.h"
#include "gamemap.h"
#include "player.h"
typedef enum GameScreen { Title = 0, GamePlayScreen } GameScreen;
int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 800;
    // C++
    // init
    raylib::Color background_color(raylib::Color::White());
    raylib::Window window(screenWidth, screenHeight, "Raylib in C++");
    raylib::Vector2 player_original_position((float) screenWidth / 3, (float) 0);
    // raylib::Texture2D background((raylib::Image) {"res/image.png"});
    GameScreen currentScreen = GameScreen::Title;
    raylib::Camera2D camera((raylib::Vector2){static_cast<float>(window.GetWidth() / 2),
                                              static_cast<float>(window.GetHeight() / 2)},
                            raylib::Vector2{player_original_position.GetX() + 20,
                                            player_original_position.GetY() + 20});
    // I have no ideal how the code bellow even works
    std::map<action, animation> player_anim;
    player_anim[action::moving] = animation("move.png", 18);
    player_anim[action::waiting] = animation("wait.png", 30);
    player_anim[action::attacking] = animation("attack.png", 25);
    // Game objects
    Player player(player_original_position, 1, true, raylib::Color::Black(), player_anim);
    GameMap map;
    std::vector<Enemies> enemies;
    enemies.emplace_back((raylib::Vector2){800, 0}, 50, 400);
    enemies.emplace_back((raylib::Vector2){100, 0}, 50, 400);
    enemies.emplace_back((raylib::Vector2){1000, 0}, 50, 400);
    std::vector<Enemies> orig_enemies = enemies;
    window.SetTargetFPS(60);
    // Main loop
    while (!window.ShouldClose()) {
        switch (currentScreen) {
        case Title: {
            if (raylib::Keyboard::IsKeyPressed(KEY_ENTER)
                || raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON))
                currentScreen = GamePlayScreen;
            player.position = player_original_position;
            enemies = orig_enemies;
            break;
        }
        case GamePlayScreen: {
            // Game screen
            if (raylib::Keyboard::IsKeyPressed(KEY_R))
                currentScreen = GameScreen::Title;
            float delta = window.GetFrameTime();
            player.updatePlayer(&map.map_objects, &enemies, delta);
            player.updateCamera(&camera, window.GetWidth(), window.GetHeight());
            for (size_t i = 0; i < enemies.size(); ++i) {
                auto &enemy(enemies.at(i));
                if (enemies.at(i).isAlive) {
                    enemy.update(&player, &map.map_objects, delta);
                } else {
                    enemies.erase(enemies.begin() + i); // feel dangerous, probably fine?
                }
            }
            raylib::DrawText("Enemies count: " + std::to_string(enemies.size()), 10, 10, 25, BLACK);
        }
        }
        BeginDrawing();
        {
            window.ClearBackground(background_color);
            switch (currentScreen) {
            case Title: {
                raylib::Text Text;
                Text.Draw("Raylib with C++,\n move with arrow keys\n"
                          "Press Enter to move to play screen\n"
                          "Or click the screen",
                          screenWidth / 4,
                          window.GetHeight() / 2,
                          20,
                          raylib::Color::Black());
                break;
            }
            case GamePlayScreen: {
                // BeginMode2D(camera);
                camera.BeginMode();
                // background.Draw(0, 0);
                map.draw_map();
                for (auto &enemy : enemies) {
                    enemy.draw();
                }
                player.draw_player();
                // EndMode2D();
                camera.EndMode();
                break;
            }
            }
        }
        EndDrawing();
    }
    return 0;
}
