#include "task_manager.hpp"
#include <iostream>

int main() {
    TaskManager manager;
    manager.load_from_file("tasks.txt");

    std::string command;
    while (true) {
        std::cout << "\nCommands: add | list | done | delete | exit\n> ";
        std::cin >> command;

        if (command == "add") {
            std::string title, date;
            std::cout << "Title: "; std::cin.ignore(); std::getline(std::cin, title);
            std::cout << "Due date: "; std::getline(std::cin, date);
            manager.add_task(title, date);
        } else if (command == "list") {
            manager.list_tasks();
        } else if (command == "done") {
            int id;
            std::cout << "Task ID: "; std::cin >> id;
            manager.mark_done(id);
        } else if (command == "delete") {
            int id;
            std::cout << "Task ID: "; std::cin >> id;
            manager.delete_task(id);
        } else if (command == "exit") {
            manager.save_to_file("tasks.txt");
            break;
        } else {
            std::cout << "Unknown command.\n";
        }
    }
}
