#pragma once
#include <string>
#include <memory>
#include "Class/Template/Collection.h"
#include "Class/Base/Header/Human.h"

class Human_loader {
public:
    static void save(const std::string& path,
                     const Collection<std::unique_ptr<Human>>& humans);

    static Collection<std::unique_ptr<Human>> load(const std::string& path);
};