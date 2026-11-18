#pragma once
#include "Class/Exception/Header/Greenhouse_exception.h"
#include <string>
#include <cstddef>
#include <sstream>

class File_format_exception : public Greenhouse_exception {
private:
    std::string path_;
    std::size_t line_number_;
    std::string details_;
public:
    File_format_exception(const std::string& path,
                          std::size_t line_number,
                          const std::string& details)
        : Greenhouse_exception(make_message(path, line_number, details))
        , path_(path)
        , line_number_(line_number)
        , details_(details)
    {}

    const std::string& path()        const noexcept { return path_; }
    std::size_t        line_number() const noexcept { return line_number_; }
    const std::string& details()     const noexcept { return details_; }

private:
    static std::string make_message(const std::string& path,
                                    std::size_t line,
                                    const std::string& details) {
        std::ostringstream os;
        os << "Format error in file \"" << path
           << "\", line " << line << ": " << details;
        return os.str();
    }
};