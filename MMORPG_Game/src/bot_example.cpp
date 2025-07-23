#include "entities/Bot.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    try {
        // Create a default configuration
        BotConfig config = BotConfig::getDefault();
        
        // Optionally load configuration from file
        // config = BotConfig::loadFromFile("bot_config.json");
        
        // Create a bot with Warrior class
        Bot bot("AutoWarrior", CharacterClass::WARRIOR, config);
        
        std::cout << "Bot created. Starting in 3 seconds...\n";
        std::cout << "Make sure the game window is active!\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        
        // Start the bot
        bot.startBot();
        
        std::cout << "Bot is running. Press Enter to stop...\n";
        std::cin.get();
        
        // Stop the bot
        bot.stopBot();
        std::cout << "Bot stopped.\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 