#include "Class/Manager/File_manager/Header/Equipment_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include "Class/Inheritor/Inheritor2/Permanent_Equipment/Header/Lighting.h"
#include "Class/Inheritor/Inheritor2/Single_use_Equipment/Header/Watering_Equipment.h"
#include "Class/Inheritor/Inheritor2/Single_use_Equipment/Header/Fertilizer_Equipment.h"
#include <fstream>
#include <sstream>

void Equipment_loader::save(const std::string& path,
                            const Collection<std::unique_ptr<Equipment>>& equipment) {
    std::ofstream file(path);
    if (!file.is_open())
        throw File_open_exception(path, "writing");

    for (std::size_t i = 0; i < equipment.size(); ++i) {
        const Equipment* eq = equipment[i].get();
        file << eq->get_type() << ";"
             << eq->get_name() << ";"
             << eq->get_energy_requirement() << ";"
             << eq->get_is_broken() << ";"
             << eq->get_wear_level();

        if (eq->get_type() == "Watering_equipment" ||
            eq->get_type() == "Fertilizer_equipment") {
            const Single_use_equipment* single =
                dynamic_cast<const Single_use_equipment*>(eq);
            file << ";" << single->get_resource_left();
        }
        file << "\n";
    }
}

Collection<std::unique_ptr<Equipment>> Equipment_loader::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw File_open_exception(path, "reading");

    Collection<std::unique_ptr<Equipment>> result;
    std::string line;
    std::size_t line_num = 0;

    while (std::getline(file, line)) {
        line_num++;
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string type, name, energy_str, broken_str, wear_str, res_str;

        if (!std::getline(ss, type, ';') ||
            !std::getline(ss, name, ';') ||
            !std::getline(ss, energy_str, ';') ||
            !std::getline(ss, broken_str, ';') ||
            !std::getline(ss, wear_str, ';')) {
            throw File_format_exception(path, line_num, "invalid equipment column count");
        }

        try {
            double energy = std::stod(energy_str);
            bool broken   = (broken_str == "1");
            double wear   = std::stod(wear_str);

            std::unique_ptr<Equipment> eq;
            if (type == "Lighting") {
                eq = std::make_unique<Lighting>(name, energy, 50.0, 550.0);
            } else if (type == "Watering_equipment") {
                if (!std::getline(ss, res_str, ';'))
                    throw File_format_exception(path, line_num, "missing resource field");
                eq = std::make_unique<Watering_equipment>(name, energy, std::stod(res_str), 1.0);
            } else if (type == "Fertilizer_equipment") {
                if (!std::getline(ss, res_str, ';'))
                    throw File_format_exception(path, line_num, "missing resource field");
                eq = std::make_unique<Fertilizer_equipment>(name, energy, std::stod(res_str), 1.0);
            } else {
                throw File_format_exception(path, line_num, "unknown equipment type");
            }

            eq->set_is_broken(broken);
            eq->set_wear_level(wear);
            result.push_back(std::move(eq));
        } catch (const File_format_exception&) {
            throw;
        } catch (const std::exception&) {
            throw File_format_exception(path, line_num, "invalid numeric value");
        }
    }
    return result;
}