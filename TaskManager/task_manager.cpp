#include "task_manager.hpp"
#include <fstream>
#include <iostream>

void TaskManager::add_task(const std::string& title, const std::string& due_date) {
    tasks.emplace_back(next_id++, title, due_date);
    std::cout << "Task added.\n";
}

void TaskManager::list_tasks() const {
    for (const auto& task : tasks) {
        std::cout << "[" << task.id << "] "
                  << (task.completed ? "[x] " : "[ ] ")
                  << task.title << " (Due: " << task.due_date << ")\n";
    }
}

void TaskManager::mark_done(int id) {
    for (auto& task : tasks) {
        if (task.id == id) {
            task.completed = true;
            std::cout << "Task marked as done.\n";
            return;
        }
    }
    std::cout << "Task not found.\n";
}

void TaskManager::delete_task(int id) {
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->id == id) {
            tasks.erase(it);
            std::cout << "Task deleted.\n";
            return;
        }
    }
    std::cout << "Task not found.\n";
}

void TaskManager::save_to_file(const std::string& filename) {
    std::ofstream out(filename);
    for (const auto& task : tasks) {
        out << task.serialize() << "\n";
    }
}

void TaskManager::load_from_file(const std::string& filename) {
    std::ifstream in(filename);
    std::string line;
    while (std::getline(in, line)) {
        Task task = Task::deserialize(line);
        tasks.push_back(task);
        if (task.id >= next_id) next_id = task.id + 1;
    }
}
