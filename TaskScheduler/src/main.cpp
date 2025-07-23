#include "../include/TaskScheduler.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>

// Helper function to print time
void printTime(const std::chrono::system_clock::time_point& time) {
    auto time_t = std::chrono::system_clock::to_time_t(time);
    std::cout << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M");
}

// Helper function to print task details
void printTask(const std::shared_ptr<Task>& task) {
    std::cout << "Task: " << task->getName() << "\n";
    std::cout << "Duration: " << task->getDuration().count() << " minutes\n";
    std::cout << "Priority: ";
    switch (task->getPriority()) {
        case Priority::LOW: std::cout << "Low"; break;
        case Priority::MEDIUM: std::cout << "Medium"; break;
        case Priority::HIGH: std::cout << "High"; break;
        case Priority::URGENT: std::cout << "Urgent"; break;
    }
    std::cout << "\nDeadline: ";
    printTime(task->getDeadline());
    std::cout << "\nScheduled: ";
    printTime(task->getScheduledTime());
    std::cout << "\nCompleted: " << (task->isCompleted() ? "Yes" : "No") << "\n\n";
}

int main() {
    TaskScheduler scheduler;
    
    // Create some example tasks
    auto now = std::chrono::system_clock::now();
    
    auto task1 = std::make_shared<Task>(
        "Complete Project Report",
        std::chrono::minutes(120),
        Priority::HIGH,
        now + std::chrono::hours(24)
    );
    
    auto task2 = std::make_shared<Task>(
        "Team Meeting",
        std::chrono::minutes(60),
        Priority::URGENT,
        now + std::chrono::hours(4)
    );
    
    auto task3 = std::make_shared<Task>(
        "Code Review",
        std::chrono::minutes(90),
        Priority::MEDIUM,
        now + std::chrono::hours(48)
    );
    
    // Add tasks to scheduler
    scheduler.addTask(task1);
    scheduler.addTask(task2);
    scheduler.addTask(task3);
    
    // Add a calendar event
    scheduler.addCalendarEvent(
        now + std::chrono::hours(2),
        now + std::chrono::hours(3),
        "Lunch Break"
    );
    
    // Schedule tasks
    scheduler.scheduleTasks();
    
    // Print all tasks
    std::cout << "=== All Tasks ===\n";
    for (const auto& task : {task1, task2, task3}) {
        printTask(task);
    }
    
    // Print tasks by priority
    std::cout << "=== Urgent Tasks ===\n";
    auto urgentTasks = scheduler.getTasksByPriority(Priority::URGENT);
    for (const auto& task : urgentTasks) {
        printTask(task);
    }
    
    // Mark a task as completed and reschedule
    task2->setCompleted(true);
    scheduler.rescheduleTasks();
    
    std::cout << "=== After Rescheduling ===\n";
    for (const auto& task : {task1, task2, task3}) {
        printTask(task);
    }
    
    return 0;
} 