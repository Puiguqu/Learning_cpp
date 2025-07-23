#include "../include/TaskScheduler.hpp"
#include <algorithm>
#include <iostream>

TaskScheduler::TaskScheduler() {}

void TaskScheduler::addTask(std::shared_ptr<Task> task) {
    tasks_.push_back(task);
    sortTasksByPriority();
}

void TaskScheduler::removeTask(const std::string& taskName) {
    tasks_.erase(
        std::remove_if(tasks_.begin(), tasks_.end(),
            [&taskName](const std::shared_ptr<Task>& task) {
                return task->getName() == taskName;
            }),
        tasks_.end()
    );
}

void TaskScheduler::updateTask(const std::string& taskName, std::shared_ptr<Task> newTask) {
    auto it = std::find_if(tasks_.begin(), tasks_.end(),
        [&taskName](const std::shared_ptr<Task>& task) {
            return task->getName() == taskName;
        });
    
    if (it != tasks_.end()) {
        *it = newTask;
        sortTasksByPriority();
    }
}

void TaskScheduler::scheduleTasks() {
    auto now = std::chrono::system_clock::now();
    
    for (auto& task : tasks_) {
        if (!task->isCompleted()) {
            auto scheduledTime = findNextAvailableTimeSlot(now, task->getDuration());
            task->setScheduledTime(scheduledTime);
            now = scheduledTime + task->getDuration();
        }
    }
}

void TaskScheduler::rescheduleTasks() {
    // Clear all scheduled times
    for (auto& task : tasks_) {
        task->setScheduledTime(std::chrono::system_clock::time_point::min());
    }
    
    // Reschedule all tasks
    scheduleTasks();
}

std::vector<std::shared_ptr<Task>> TaskScheduler::getTasksByPriority(Priority priority) const {
    std::vector<std::shared_ptr<Task>> result;
    std::copy_if(tasks_.begin(), tasks_.end(), std::back_inserter(result),
        [priority](const std::shared_ptr<Task>& task) {
            return task->getPriority() == priority;
        });
    return result;
}

std::vector<std::shared_ptr<Task>> TaskScheduler::getTasksByDate(
    const std::chrono::system_clock::time_point& date) const {
    std::vector<std::shared_ptr<Task>> result;
    std::copy_if(tasks_.begin(), tasks_.end(), std::back_inserter(result),
        [&date](const std::shared_ptr<Task>& task) {
            auto taskDate = std::chrono::system_clock::to_time_t(task->getScheduledTime());
            auto targetDate = std::chrono::system_clock::to_time_t(date);
            return std::chrono::system_clock::from_time_t(taskDate) == 
                   std::chrono::system_clock::from_time_t(targetDate);
        });
    return result;
}

std::vector<std::shared_ptr<Task>> TaskScheduler::getOverdueTasks() const {
    auto now = std::chrono::system_clock::now();
    std::vector<std::shared_ptr<Task>> result;
    std::copy_if(tasks_.begin(), tasks_.end(), std::back_inserter(result),
        [&now](const std::shared_ptr<Task>& task) {
            return !task->isCompleted() && task->getDeadline() < now;
        });
    return result;
}

void TaskScheduler::addCalendarEvent(
    const std::chrono::system_clock::time_point& start,
    const std::chrono::system_clock::time_point& end,
    const std::string& description) {
    calendar_events_[start] = description;
}

void TaskScheduler::removeCalendarEvent(const std::string& description) {
    for (auto it = calendar_events_.begin(); it != calendar_events_.end();) {
        if (it->second == description) {
            it = calendar_events_.erase(it);
        } else {
            ++it;
        }
    }
}

bool TaskScheduler::isTimeSlotAvailable(
    const std::chrono::system_clock::time_point& start,
    const std::chrono::minutes& duration) const {
    
    auto end = start + duration;
    
    // Check calendar events
    for (const auto& event : calendar_events_) {
        if (start < event.first && end > event.first) {
            return false;
        }
    }
    
    // Check scheduled tasks
    for (const auto& task : tasks_) {
        if (task->getScheduledTime() != std::chrono::system_clock::time_point::min()) {
            auto taskEnd = task->getScheduledTime() + task->getDuration();
            if (start < taskEnd && end > task->getScheduledTime()) {
                return false;
            }
        }
    }
    
    return true;
}

std::chrono::system_clock::time_point TaskScheduler::findNextAvailableTimeSlot(
    const std::chrono::system_clock::time_point& start,
    const std::chrono::minutes& duration) const {
    
    auto current = start;
    while (!isTimeSlotAvailable(current, duration)) {
        current += std::chrono::minutes(30); // Check every 30 minutes
    }
    return current;
}

void TaskScheduler::sortTasksByPriority() {
    std::sort(tasks_.begin(), tasks_.end(),
        [](const std::shared_ptr<Task>& a, const std::shared_ptr<Task>& b) {
            return static_cast<int>(a->getPriority()) > static_cast<int>(b->getPriority());
        });
} 