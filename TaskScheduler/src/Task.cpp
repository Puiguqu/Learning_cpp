#include "../include/Task.hpp"

Task::Task(const std::string& name,
           const std::chrono::minutes& duration,
           Priority priority,
           const std::chrono::system_clock::time_point& deadline)
    : name_(name)
    , duration_(duration)
    , priority_(priority)
    , deadline_(deadline)
    , completed_(false)
    , scheduled_time_(std::chrono::system_clock::time_point::min()) {
} 