#pragma once
#include <string>

class Task {
public:
    int id;
    std::string title;
    std::string due_date;
    bool completed;

    Task(int id, const std::string& title, const std::string& due_date);
    std::string serialize() const;
    static Task deserialize(const std::string& line);
};
