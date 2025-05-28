cmake_minimum_required(VERSION 3.10)
project(TaskScheduler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/main.cpp
    src/Task.cpp
    src/TaskScheduler.cpp
)

# Add header files
set(HEADERS
    include/Task.hpp
    include/TaskScheduler.hpp
)

# Create executable
add_executable(task_scheduler ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(task_scheduler PRIVATE include) 