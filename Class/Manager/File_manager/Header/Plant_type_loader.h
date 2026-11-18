#pragma once
#include <string>
#include <vector>
#include "Class/Manager/File_manager/Header/Plant_type_data.h"

class Plant_type_loader {
public:
    static std::vector<Plant_type_data> load(const std::string& path);
    static void save(const std::string& path,
                     const std::vector<Plant_type_data>& types);
};