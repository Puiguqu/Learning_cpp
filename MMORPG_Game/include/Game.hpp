#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <memory>
#include <string>
#include <vector>

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void initializeWindow();
    void initializeSystems();

    // Window and rendering
    sf::RenderWindow window;
    sf::View gameView;
    sf::Clock clock;

    // Game state
    bool isRunning;
    float frameTime;

    // Constants
    static constexpr int WINDOW_WIDTH = 1280;
    static constexpr int WINDOW_HEIGHT = 720;
    static constexpr float FPS = 60.0f;
    static constexpr float FRAME_TIME = 1.0f / FPS;
}; 