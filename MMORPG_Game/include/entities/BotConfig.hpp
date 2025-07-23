#pragma once

#include <string>
#include <random>
#include <nlohmann/json.hpp>

struct BotConfig {
    // Movement settings
    int moveDurationMin;
    int moveDurationMax;
    int moveDelayMin;
    int moveDelayMax;

    // Attack settings
    int attackDelayMin;
    int attackDelayMax;
    int attackCount;

    // Buff settings
    int buffInterval;
    int buffDelayMin;
    int buffDelayMax;

    // General settings
    int loopDelayMin;
    int loopDelayMax;
    int stuckDetectionTime;
    int stuckJumpCount;

    // Window settings
    std::string windowTitle;
    bool requireActiveWindow;

    // Load configuration from JSON
    static BotConfig loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

    // Default configuration
    static BotConfig getDefault() {
        return {
            // Movement
            1000, 1400,    // moveDurationMin, moveDurationMax
            200, 400,      // moveDelayMin, moveDelayMax
            
            // Attack
            250, 350,      // attackDelayMin, attackDelayMax
            3,             // attackCount
            
            // Buff
            30,            // buffInterval
            400, 600,      // buffDelayMin, buffDelayMax
            
            // General
            250, 350,      // loopDelayMin, loopDelayMax
            5000,          // stuckDetectionTime
            3,             // stuckJumpCount
            
            // Window
            "MMORPG Game", // windowTitle
            true           // requireActiveWindow
        };
    }
}; 