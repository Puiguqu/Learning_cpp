#include "Game.hpp"
#include <iostream>

Game::Game() : isRunning(false), frameTime(0.0f) {
    initializeWindow();
    initializeSystems();
}

Game::~Game() {
    window.close();
}

void Game::initializeWindow() {
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MMORPG Game");
    window.setFramerateLimit(static_cast<unsigned int>(FPS));
    
    gameView.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    gameView.setCenter(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    window.setView(gameView);
}

void Game::initializeSystems() {
    // Initialize game systems here
    isRunning = true;
}

void Game::run() {
    while (isRunning && window.isOpen()) {
        processEvents();
        
        float deltaTime = clock.restart().asSeconds();
        update(deltaTime);
        
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            isRunning = false;
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
                isRunning = false;
            }
        }
    }
}

void Game::update(float deltaTime) {
    frameTime += deltaTime;
    
    // Update game logic here
}

void Game::render() {
    window.clear(sf::Color(50, 50, 50));
    
    // Draw game objects here
    
    window.display();
} 