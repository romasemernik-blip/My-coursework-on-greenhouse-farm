#pragma once
#include <string>
#include "Class/Base/Header/Greenhouse.h"
#include "Class/Base/Header/Storage.h"
#include "Class/Manager/Time_manager/Header/Time_manager.h"

class Progress_saver {
public:
    static void save(const std::string& path,
                     const Greenhouse& greenhouse,
                     const Storage& storage,
                     const Time_manager& time_manager);

    static void load(const std::string& path,
                     Greenhouse& greenhouse,
                     Storage& storage,
                     Time_manager& time_manager);
};