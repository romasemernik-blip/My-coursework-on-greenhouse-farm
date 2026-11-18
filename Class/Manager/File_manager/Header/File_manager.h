#pragma once
#include <string>
#include <vector>
#include "Class/Manager/File_manager/Header/Plant_type_data.h"
#include "Class/Base/Header/Greenhouse.h"
#include "Class/Base/Header/Storage.h"
#include "Class/Manager/Time_manager/Header/Time_manager.h"
#include "Class/Template/Collection.h"

class File_manager {
private:
    std::vector<Plant_type_data> plant_types_;

public:
    File_manager() = default;

    void load_plant_types(const std::string& path);
    void save_plant_types(const std::string& path) const;
    const std::vector<Plant_type_data>& get_plant_types() const noexcept;

    void save_equipment(const std::string& path,
                        const Collection<std::unique_ptr<Equipment>>& equipment) const;
    Collection<std::unique_ptr<Equipment>> load_equipment(const std::string& path) const;

    void save_humans(const std::string& path,
                     const Collection<std::unique_ptr<Human>>& humans) const;
    Collection<std::unique_ptr<Human>> load_humans(const std::string& path) const;

    void save_beds(const std::string& path,
                   const Collection<std::unique_ptr<Bed>>& beds) const;
    Collection<std::unique_ptr<Bed>> load_beds(const std::string& path) const;

    void save_progress(const std::string& folder,
                       const Greenhouse& greenhouse,
                       const Storage& storage,
                       const Time_manager& time_manager) const;

    void load_progress(const std::string& folder,
                       Greenhouse& greenhouse,
                       Storage& storage,
                       Time_manager& time_manager) const;

    void load_all(const std::string& folder,
                  Greenhouse& greenhouse,
                  Storage& storage,
                  Time_manager& time_manager);

    void save_all(const std::string& folder,
                  const Greenhouse& greenhouse,
                  const Storage& storage,
                  const Time_manager& time_manager) const;
};