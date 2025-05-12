#include "task.hpp"
#include <sstream>

Task::Task(int id, const std::string& title, const std::string& due_date)
    : id(id), title(title), due_date(due_date), completed(false) {}

std::string Task::serialize() const {
    return std::to_string(id) + "," + title + "," + due_date + "," + (completed ? "1" : "0");
}

Task Task::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string id_str, title, date, completed_str;
    std::getline(ss, id_str, ',');
    std::getline(ss, title, ',');
    std::getline(ss, date, ',');
    std::getline(ss, completed_str, ',');

    Task task(std::stoi(id_str), title, date);
    task.completed = (completed_str == "1");
    return task;
}
