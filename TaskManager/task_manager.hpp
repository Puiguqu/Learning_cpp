#pragma once
#include "task.hpp"
#include <vector>
#include <string>

class TaskManager {
private:
    std::vector<Task> tasks;
    int next_id = 1;

public:
    void load_from_file(const std::string& filename);
    void save_to_file(const std::string& filename);
    void add_task(const std::string& title, const std::string& due_date);
    void list_tasks() const;
    void mark_done(int id);
    void delete_task(int id);
};
