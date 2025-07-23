#pragma once

#include "Character.hpp"
#include "BotConfig.hpp"
#include <windows.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <random>

enum class BotState {
    Idle,
    MovingLeft,
    MovingRight,
    Attacking,
    Buffing,
    Stuck
};

class Bot : public Character {
public:
    Bot(const std::string& name, CharacterClass characterClass, const BotConfig& config = BotConfig::getDefault());
    ~Bot();

    void startBot();
    void stopBot();
    bool isBotRunning() const { return running; }
    void setConfig(const BotConfig& config) { this->config = config; }
    const BotConfig& getConfig() const { return config; }

private:
    void botLoop();
    void pressKey(WORD vk, int delay_ms = 50);
    void holdKey(WORD vk, int duration_ms);
    void updateBotState();
    bool isWindowActive() const;
    bool isStuck() const;
    void handleStuckState();
    int getRandomDelay(int min, int max);

    std::atomic<bool> running;
    std::thread botThread;
    BotState currentState;
    int loopCounter;
    
    // Stuck detection
    sf::Vector2f lastPosition;
    std::chrono::steady_clock::time_point lastPositionTime;
    int stuckJumpCounter;
    
    // Random number generation
    std::mt19937 rng;
    std::uniform_int_distribution<int> moveDurationDist;
    std::uniform_int_distribution<int> moveDelayDist;
    std::uniform_int_distribution<int> attackDelayDist;
    std::uniform_int_distribution<int> buffDelayDist;
    std::uniform_int_distribution<int> loopDelayDist;
    
    // Configuration
    BotConfig config;
}; 