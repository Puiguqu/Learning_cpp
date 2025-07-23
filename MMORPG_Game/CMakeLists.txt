cmake_minimum_required(VERSION 3.10)
project(MMORPG_Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(SFML 2.5 COMPONENTS graphics window system network REQUIRED)

# Add nlohmann/json
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Add source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/entities/Player.cpp
    src/entities/Character.cpp
    src/entities/Monster.cpp
    src/entities/Bot.cpp
    src/entities/BotConfig.cpp
    src/world/Map.cpp
    src/world/World.cpp
    src/systems/CombatSystem.cpp
    src/systems/InventorySystem.cpp
    src/systems/QuestSystem.cpp
    src/network/NetworkManager.cpp
)

# Add header files
set(HEADERS
    include/Game.hpp
    include/entities/Player.hpp
    include/entities/Character.hpp
    include/entities/Monster.hpp
    include/entities/Bot.hpp
    include/entities/BotConfig.hpp
    include/world/Map.hpp
    include/world/World.hpp
    include/systems/CombatSystem.hpp
    include/systems/InventorySystem.hpp
    include/systems/QuestSystem.hpp
    include/network/NetworkManager.hpp
)

# Create main executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Create bot example executable
add_executable(bot_example src/bot_example.cpp ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(bot_example PRIVATE include)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-network
    nlohmann_json::nlohmann_json
)

target_link_libraries(bot_example PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-network
    nlohmann_json::nlohmann_json
) 