#pragma once

#include <string>
#include <chrono>
#include <ctime>

enum class Priority {
    LOW,
    MEDIUM,
    HIGH,
    URGENT
};

class Task {
public:
    Task(const std::string& name, 
         const std::chrono::minutes& duration,
         Priority priority,
         const std::chrono::system_clock::time_point& deadline);

    // Getters
    const std::string& getName() const { return name_; }
    std::chrono::minutes getDuration() const { return duration_; }
    Priority getPriority() const { return priority_; }
    std::chrono::system_clock::time_point getDeadline() const { return deadline_; }
    bool isCompleted() const { return completed_; }
    std::chrono::system_clock::time_point getScheduledTime() const { return scheduled_time_; }

    // Setters
    void setScheduledTime(const std::chrono::system_clock::time_point& time) { scheduled_time_ = time; }
    void setCompleted(bool completed) { completed_ = completed; }
    void setPriority(Priority priority) { priority_ = priority; }

private:
    std::string name_;
    std::chrono::minutes duration_;
    Priority priority_;
    std::chrono::system_clock::time_point deadline_;
    bool completed_ = false;
    std::chrono::system_clock::time_point scheduled_time_;
}; 