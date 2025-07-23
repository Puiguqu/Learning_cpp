#include "task_manager.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

void TaskManager::add_task(const std::string& title, const std::string& due_date) {
    Task task(next_id++, title, due_date);
    tasks.push_back(task);
    std::cout << "Task added successfully!\n";
}

void TaskManager::list_tasks() const {
    if (tasks.empty()) {
        std::cout << "No tasks found.\n";
        return;
    }
    
    std::cout << "\n=== TASKS ===\n";
    for (const auto& task : tasks) {
        std::cout << "ID: " << task.id 
                  << " | Title: " << task.title 
                  << " | Due: " << task.due_date 
                  << " | Status: " << (task.completed ? "DONE" : "PENDING") << "\n";
    }
    std::cout << "=============\n\n";
}

void TaskManager::mark_done(int id) {
    auto it = std::find_if(tasks.begin(), tasks.end(), 
                          [id](Task& task) { return task.id == id; });
    
    if (it != tasks.end()) {
        it->completed = true;
        std::cout << "Task marked as done!\n";
    } else {
        std::cout << "Task not found!\n";
    }
}

void TaskManager::delete_task(int id) {
    auto it = std::find_if(tasks.begin(), tasks.end(), 
                          [id](const Task& task) { return task.id == id; });
    
    if (it != tasks.end()) {
        tasks.erase(it);
        std::cout << "Task deleted!\n";
    } else {
        std::cout << "Task not found!\n";
    }
}

void TaskManager::save_to_file(const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cout << "Error: Could not save to file!\n";
        return;
    }
    
    for (const auto& task : tasks) {
        out << task.serialize() << "\n";
    }
    std::cout << "Tasks saved to " << filename << "\n";
}

void TaskManager::load_from_file(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "No existing task file found. Starting fresh.\n";
        return;
    }
    
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) {
            Task task = Task::deserialize(line);
            tasks.push_back(task);
            if (task.id >= next_id) next_id = task.id + 1;
        }
    }
    std::cout << "Tasks loaded from " << filename << "\n";
}