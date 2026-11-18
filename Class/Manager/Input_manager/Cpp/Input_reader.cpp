#include "Class/Manager/Input_manager/Header/Input_reader.h"
#include "Class/Exception/Header/All_exception.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>

std::string Input_reader::read_line(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line))
        throw File_open_exception("stdin", "reading");
    return line;
}

int Input_reader::read_int(const std::string& prompt,
                           int min_value,
                           int max_value,
                           bool allow_cancel,
                           int cancel_value) {
    while (true) {
        std::string line = read_line(prompt);
        if (allow_cancel && line == "cancel")
            return cancel_value;

        std::stringstream ss(line);
        int value;
        if (!(ss >> value))
            throw Invalid_argument_exception("input", "not an integer: " + line);

        std::string rest;
        if (ss >> rest)
            throw Invalid_argument_exception("input",
                "unexpected trailing characters: '" + rest + "'");

        if (value < min_value || value > max_value)
            throw Invalid_argument_exception("input",
                "value out of range: " + std::to_string(value));

        return value;
    }
}

std::size_t Input_reader::read_size_t(const std::string& prompt,
                                      std::size_t min_value,
                                      std::size_t max_value,
                                      bool allow_cancel,
                                      std::size_t cancel_value) {
    while (true) {
        std::string line = read_line(prompt);
        if (allow_cancel && line == "cancel")
            return cancel_value;

        std::stringstream ss(line);
        std::size_t value;
        if (!(ss >> value))
            throw Invalid_argument_exception("input", "not a positive integer: " + line);

        std::string rest;
        if (ss >> rest)
            throw Invalid_argument_exception("input",
                "unexpected trailing characters: '" + rest + "'");

        if (value < min_value || value > max_value)
            throw Invalid_argument_exception("input",
                "value out of range: " + std::to_string(value));

        return value;
    }
}

double Input_reader::read_double(const std::string& prompt,
                                 double min_value,
                                 double max_value,
                                 bool allow_cancel,
                                 double cancel_value) {
    while (true) {
        std::string line = read_line(prompt);
        if (allow_cancel && line == "cancel")
            return cancel_value;

        std::stringstream ss(line);
        double value;
        if (!(ss >> value))
            throw Invalid_argument_exception("input", "not a number: " + line);

        std::string rest;
        if (ss >> rest)
            throw Invalid_argument_exception("input",
                "unexpected trailing characters: '" + rest + "'");

        if (value < min_value || value > max_value)
            throw Invalid_argument_exception("input",
                "value out of range: " + std::to_string(value));

        return value;
    }
}

std::string Input_reader::read_string(const std::string& prompt,
                                      bool allow_empty,
                                      bool allow_cancel,
                                      const std::string& cancel_value) {
    while (true) {
        std::string line = read_line(prompt);
        if (allow_cancel && line == "cancel")
            return cancel_value;

        if (!allow_empty && line.empty())
            throw Invalid_argument_exception("input", "string must not be empty");

        return line;
    }
}

bool Input_reader::read_bool(const std::string& prompt,
                             bool allow_cancel,
                             bool cancel_value) {
    while (true) {
        std::string line = read_line(prompt);
        if (allow_cancel && line == "cancel")
            return cancel_value;

        std::transform(line.begin(), line.end(), line.begin(), ::tolower);

        if (line == "y" || line == "yes" || line == "1" || line == "true")
            return true;
        if (line == "n" || line == "no" || line == "0" || line == "false")
            return false;

        throw Invalid_argument_exception("input",
            "not a boolean value: '" + line + "' (use y/n)");
    }
}

int Input_reader::read_choice(const std::string& prompt,
                              const std::string& options,
                              int min_value,
                              int max_value,
                              bool allow_cancel,
                              int cancel_value) {
    std::cout << options << "\n";
    return read_int(prompt, min_value, max_value, allow_cancel, cancel_value);
}