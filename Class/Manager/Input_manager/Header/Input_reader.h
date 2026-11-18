#pragma once
#include <string>
#include <cstddef>

class Input_reader {
public:
    static int read_int(const std::string& prompt,
                        int min_value = -2147483647,
                        int max_value =  2147483647,
                        bool allow_cancel = false,
                        int cancel_value = -1);

    static std::size_t read_size_t(const std::string& prompt,
                                   std::size_t min_value = 0,
                                   std::size_t max_value = static_cast<std::size_t>(-1),
                                   bool allow_cancel = false,
                                   std::size_t cancel_value = 0);

    static double read_double(const std::string& prompt,
                              double min_value = -1e300,
                              double max_value =  1e300,
                              bool allow_cancel = false,
                              double cancel_value = -1.0);

    static std::string read_string(const std::string& prompt,
                                   bool allow_empty = false,
                                   bool allow_cancel = false,
                                   const std::string& cancel_value = "");

    static bool read_bool(const std::string& prompt,
                          bool allow_cancel = false,
                          bool cancel_value = false);

    static int read_choice(const std::string& prompt,
                           const std::string& options,
                           int min_value,
                           int max_value,
                           bool allow_cancel = false,
                           int cancel_value = -1);

private:
    static std::string read_line(const std::string& prompt);
};