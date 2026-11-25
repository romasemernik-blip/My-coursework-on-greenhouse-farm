#include "Class/Manager/File_manager/Header/File_manager.h"
#include "Class/Manager/File_manager/Header/Plant_type_loader.h"
#include "Class/Manager/File_manager/Header/Equipment_loader.h"
#include "Class/Manager/File_manager/Header/Human_loader.h"
#include "Class/Manager/File_manager/Header/Bed_loader.h"
#include "Class/Manager/File_manager/Header/Progress_saver.h"

void File_manager::load_plant_types(const std::string& path) {
    plant_types_ = Plant_type_loader::load(path);
}

void File_manager::save_plant_types(const std::string& path) const {
    Plant_type_loader::save(path, plant_types_);
}

const std::vector<Plant_type_data>& File_manager::get_plant_types() const noexcept {
    return plant_types_;
}

void File_manager::save_equipment(const std::string& path, const Collection<std::unique_ptr<Equipment>>& equipment) const {
    Equipment_loader::save(path, equipment);
}

Collection<std::unique_ptr<Equipment>> File_manager::load_equipment(const std::string& path) const {
    return Equipment_loader::load(path);
}

void File_manager::save_humans(const std::string& path, const Collection<std::unique_ptr<Human>>& humans) const {
    Human_loader::save(path, humans);
}

Collection<std::unique_ptr<Human>> File_manager::load_humans(const std::string& path) const {
    return Human_loader::load(path);
}

void File_manager::save_beds(const std::string& path, const Collection<std::unique_ptr<Bed>>& beds) const {
    Bed_loader::save(path, beds);
}

Collection<std::unique_ptr<Bed>> File_manager::load_beds(const std::string& path) const {
    return Bed_loader::load(path);
}

void File_manager::save_progress(const std::string& folder, const Greenhouse& greenhouse, const Storage& storage, const Time_manager& time_manager) const {
    Progress_saver::save(folder + "/progress.txt", greenhouse, storage, time_manager);
    save_beds(folder + "/beds.txt", greenhouse.get_beds());
    save_equipment(folder + "/equipment.txt", greenhouse.get_equipment_collection());
}

void File_manager::load_progress(const std::string& folder, Greenhouse& greenhouse, Storage& storage, Time_manager& time_manager) const {
    Progress_saver::load(folder + "/progress.txt", greenhouse, storage, time_manager);

    auto beds = load_beds(folder + "/beds.txt");
    for (auto& bed : beds) {
        greenhouse.add_bed(std::move(bed));
    }

    auto equipment = load_equipment(folder + "/equipment.txt");
    for (auto& eq : equipment) {
        greenhouse.add_equipment(std::move(eq));
    }
}

void File_manager::load_all(const std::string& folder, Greenhouse& greenhouse, Storage& storage, Time_manager& time_manager) {
    load_plant_types(folder + "/plant_types.txt");
    load_humans(folder + "/humans.txt"); 
    load_progress(folder, greenhouse, storage, time_manager);
}

void File_manager::save_all(const std::string& folder, const Greenhouse& greenhouse, const Storage& storage, const Time_manager& time_manager) const {
    save_plant_types(folder + "/plant_types.txt");
    save_progress(folder, greenhouse, storage, time_manager);
}