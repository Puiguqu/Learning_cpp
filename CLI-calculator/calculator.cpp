#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>

class calculator {
public:
    calculator();
    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);
};

calculator ::calculator() {
    // Constructor implementation (if needed)
}

double calculator ::add(double a, double b) {
    return a + b;
}

double calculator ::subtract(double a, double b) {
    return a - b;
}

double calculator ::multiply(double a, double b) {
    return a * b;
}

double calculator ::divide(double a, double b) {
    if (b == 0) {
        throw std::runtime_error("Division by zero is not allowed.");
    }
    return a / b;
}
