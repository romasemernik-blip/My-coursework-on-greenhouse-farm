#pragma once
#include "Class/Exception/Header/Greenhouse_exception.h"
#include <string>

class File_open_exception : public Greenhouse_exception {
private:
    std::string path_;
    std::string mode_;
public:
    File_open_exception(const std::string& path, const std::string& mode)
        : Greenhouse_exception(
              "Failed to open file \"" + path + "\" for " + mode)
        , path_(path)
        , mode_(mode)
    {}

    const std::string& path() const noexcept { return path_; }
    const std::string& mode() const noexcept { return mode_; }
};