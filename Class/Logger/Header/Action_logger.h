#pragma once
#include <string>
#include <fstream>

class Action_logger {
private:
    std::ofstream file_;
public:
    explicit Action_logger(const std::string& filepath); 
    ~Action_logger();
    void log_action(const std::string& role, const std::string& name, const std::string& action);
};