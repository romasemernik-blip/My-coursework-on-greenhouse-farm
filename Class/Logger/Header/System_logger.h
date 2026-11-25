#pragma once
#include <string>
#include <fstream>

class System_logger {
private:
    std::ofstream file_;
public:
    System_logger(const std::string& filepath);
    ~System_logger();
    void log(const std::string& message);
};