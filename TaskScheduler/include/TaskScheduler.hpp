#pragma once

#include "Task.hpp"
#include <vector>
#include <memory>
#include <map>
#include <chrono>

class TaskScheduler {
public:
    TaskScheduler();

    // Task Management
    void addTask(std::shared_ptr<Task> task);
    void removeTask(const std::string& taskName);
    void updateTask(const std::string& taskName, std::shared_ptr<Task> newTask);
    
    // Scheduling
    void scheduleTasks();
    void rescheduleTasks();
    
    // Queries
    std::vector<std::shared_ptr<Task>> getTasksByPriority(Priority priority) const;
    std::vector<std::shared_ptr<Task>> getTasksByDate(const std::chrono::system_clock::time_point& date) const;
    std::vector<std::shared_ptr<Task>> getOverdueTasks() const;
    
    // Calendar Management
    void addCalendarEvent(const std::chrono::system_clock::time_point& start,
                         const std::chrono::system_clock::time_point& end,
                         const std::string& description);
    void removeCalendarEvent(const std::string& description);

private:
    std::vector<std::shared_ptr<Task>> tasks_;
    std::map<std::chrono::system_clock::time_point, std::string> calendar_events_;
    
    // Helper methods
    bool isTimeSlotAvailable(const std::chrono::system_clock::time_point& start,
                            const std::chrono::minutes& duration) const;
    std::chrono::system_clock::time_point findNextAvailableTimeSlot(
        const std::chrono::system_clock::time_point& start,
        const std::chrono::minutes& duration) const;
    void sortTasksByPriority();
}; 