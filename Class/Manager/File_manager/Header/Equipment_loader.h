#pragma once
#include <string>
#include <memory>
#include "Class/Template/Collection.h"
#include "Class/Base/Header/Equipment.h"

class Equipment_loader {
public:
    static void save(const std::string& path,
                     const Collection<std::unique_ptr<Equipment>>& equipment);

    static Collection<std::unique_ptr<Equipment>> load(const std::string& path);
};