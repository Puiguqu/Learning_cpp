#include "entities/Bot.hpp"
#include <iostream>
#include <chrono>

Bot::Bot(const std::string& name, CharacterClass characterClass, const BotConfig& config)
    : Character(name, characterClass)
    , running(false)
    , currentState(BotState::MovingRight)
    , loopCounter(0)
    , stuckJumpCounter(0)
    , config(config)
    , rng(std::random_device{}())
    , moveDurationDist(config.moveDurationMin, config.moveDurationMax)
    , moveDelayDist(config.moveDelayMin, config.moveDelayMax)
    , attackDelayDist(config.attackDelayMin, config.attackDelayMax)
    , buffDelayDist(config.buffDelayMin, config.buffDelayMax)
    , loopDelayDist(config.loopDelayMin, config.loopDelayMax)
{
    lastPosition = getPosition();
    lastPositionTime = std::chrono::steady_clock::now();
}

Bot::~Bot() {
    stopBot();
}

void Bot::startBot() {
    if (!running) {
        running = true;
        botThread = std::thread(&Bot::botLoop, this);
    }
}

void Bot::stopBot() {
    if (running) {
        running = false;
        if (botThread.joinable()) {
            botThread.join();
        }
    }
}

void Bot::botLoop() {
    std::cout << "Starting bot for character: " << getName() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    while (running) {
        if (config.requireActiveWindow && !isWindowActive()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }

        if (isStuck()) {
            handleStuckState();
        } else {
            updateBotState();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(getRandomDelay(config.loopDelayMin, config.loopDelayMax)));
    }
}

bool Bot::isWindowActive() const {
    HWND activeWindow = GetForegroundWindow();
    if (!activeWindow) return false;

    char windowTitle[256];
    GetWindowTextA(activeWindow, windowTitle, sizeof(windowTitle));
    return std::string(windowTitle) == config.windowTitle;
}

bool Bot::isStuck() const {
    auto currentTime = std::chrono::steady_clock::now();
    auto timeSinceLastMove = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - lastPositionTime).count();

    return timeSinceLastMove > config.stuckDetectionTime &&
           getPosition() == lastPosition;
}

void Bot::handleStuckState() {
    std::cout << "[Bot] Detected stuck state, attempting to recover...\n";
    
    // Jump multiple times to try to get unstuck
    for (int i = 0; i < config.stuckJumpCount; ++i) {
        pressKey(VK_SPACE);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // Update position tracking
    lastPosition = getPosition();
    lastPositionTime = std::chrono::steady_clock::now();
    stuckJumpCounter = 0;
}

void Bot::pressKey(WORD vk, int delay_ms) {
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    SendInput(1, &input, sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void Bot::holdKey(WORD vk, int duration_ms) {
    keybd_event(vk, 0, 0, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
    keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);
}

int Bot::getRandomDelay(int min, int max) {
    return std::uniform_int_distribution<int>(min, max)(rng);
}

void Bot::updateBotState() {
    // Update position tracking
    lastPosition = getPosition();
    lastPositionTime = std::chrono::steady_clock::now();

    switch (currentState) {
        case BotState::MovingRight:
            std::cout << "[Bot] Moving Right\n";
            holdKey('D', getRandomDelay(config.moveDurationMin, config.moveDurationMax));
            pressKey(VK_SPACE);
            pressKey('X');
            currentState = BotState::Attacking;
            break;

        case BotState::MovingLeft:
            std::cout << "[Bot] Moving Left\n";
            holdKey('A', getRandomDelay(config.moveDurationMin, config.moveDurationMax));
            pressKey(VK_SPACE);
            pressKey('X');
            currentState = BotState::Attacking;
            break;

        case BotState::Attacking:
            std::cout << "[Bot] Attacking\n";
            for (int i = 0; i < config.attackCount; ++i) {
                pressKey('X');
                std::this_thread::sleep_for(std::chrono::milliseconds(
                    getRandomDelay(config.attackDelayMin, config.attackDelayMax)));
            }
            loopCounter++;
            if (loopCounter % 2 == 0)
                currentState = BotState::MovingLeft;
            else
                currentState = BotState::MovingRight;
            break;

        case BotState::Buffing:
            std::cout << "[Bot] Buffing\n";
            pressKey('C');
            std::this_thread::sleep_for(std::chrono::milliseconds(
                getRandomDelay(config.buffDelayMin, config.buffDelayMax)));
            currentState = BotState::Idle;
            break;

        case BotState::Idle:
        default:
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            break;
    }

    // Buff every config.buffInterval loops
    if (loopCounter % config.buffInterval == 0 && loopCounter > 0) {
        currentState = BotState::Buffing;
    }
} 