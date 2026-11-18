#pragma once
#include <string>
#include <memory>
#include "Class/Template/Collection.h"
#include "Class/Base/Header/Bed.h"

class Bed_loader {
public:
    static void save(const std::string& path,
                     const Collection<std::unique_ptr<Bed>>& beds);

    static Collection<std::unique_ptr<Bed>> load(const std::string& path);
};