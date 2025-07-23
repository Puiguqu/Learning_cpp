#include "entities/BotConfig.hpp"
#include <fstream>
#include <iostream>

BotConfig BotConfig::loadFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open config file: " << filename << std::endl;
            return getDefault();
        }

        nlohmann::json j;
        file >> j;

        BotConfig config;
        // Movement settings
        config.moveDurationMin = j["movement"]["durationMin"].get<int>();
        config.moveDurationMax = j["movement"]["durationMax"].get<int>();
        config.moveDelayMin = j["movement"]["delayMin"].get<int>();
        config.moveDelayMax = j["movement"]["delayMax"].get<int>();

        // Attack settings
        config.attackDelayMin = j["attack"]["delayMin"].get<int>();
        config.attackDelayMax = j["attack"]["delayMax"].get<int>();
        config.attackCount = j["attack"]["count"].get<int>();

        // Buff settings
        config.buffInterval = j["buff"]["interval"].get<int>();
        config.buffDelayMin = j["buff"]["delayMin"].get<int>();
        config.buffDelayMax = j["buff"]["delayMax"].get<int>();

        // General settings
        config.loopDelayMin = j["general"]["loopDelayMin"].get<int>();
        config.loopDelayMax = j["general"]["loopDelayMax"].get<int>();
        config.stuckDetectionTime = j["general"]["stuckDetectionTime"].get<int>();
        config.stuckJumpCount = j["general"]["stuckJumpCount"].get<int>();

        // Window settings
        config.windowTitle = j["window"]["title"].get<std::string>();
        config.requireActiveWindow = j["window"]["requireActive"].get<bool>();

        return config;
    } catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        return getDefault();
    }
}

void BotConfig::saveToFile(const std::string& filename) const {
    try {
        nlohmann::json j;
        
        // Movement settings
        j["movement"]["durationMin"] = moveDurationMin;
        j["movement"]["durationMax"] = moveDurationMax;
        j["movement"]["delayMin"] = moveDelayMin;
        j["movement"]["delayMax"] = moveDelayMax;

        // Attack settings
        j["attack"]["delayMin"] = attackDelayMin;
        j["attack"]["delayMax"] = attackDelayMax;
        j["attack"]["count"] = attackCount;

        // Buff settings
        j["buff"]["interval"] = buffInterval;
        j["buff"]["delayMin"] = buffDelayMin;
        j["buff"]["delayMax"] = buffDelayMax;

        // General settings
        j["general"]["loopDelayMin"] = loopDelayMin;
        j["general"]["loopDelayMax"] = loopDelayMax;
        j["general"]["stuckDetectionTime"] = stuckDetectionTime;
        j["general"]["stuckJumpCount"] = stuckJumpCount;

        // Window settings
        j["window"]["title"] = windowTitle;
        j["window"]["requireActive"] = requireActiveWindow;

        std::ofstream file(filename);
        file << j.dump(4);
    } catch (const std::exception& e) {
        std::cerr << "Error saving config: " << e.what() << std::endl;
    }
} 