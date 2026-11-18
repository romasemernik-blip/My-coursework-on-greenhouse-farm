#pragma once
#include <string>
#include <functional>
#include <iostream>

class Safe_input {
public:
    template<typename T>
    static T repeat_until_valid(const std::string& prompt,
                                std::function<T(const std::string&)> reader,
                                const std::string& error_prefix = "[!] ") {
        while (true) {
            try {
                return reader(prompt);
            } catch (const std::exception& e) {
                std::cout << error_prefix << e.what() << "\n";
            }
        }
    }
};