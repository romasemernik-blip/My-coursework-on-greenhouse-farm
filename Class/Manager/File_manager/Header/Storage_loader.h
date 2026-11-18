#pragma once
#include <string>
#include "Class/Base/Header/Storage.h"

class Storage_loader {
public:
    static void save_to_file(const std::string& path, const Storage& storage);
    static void load_from_file(const std::string& path, Storage& storage);
};