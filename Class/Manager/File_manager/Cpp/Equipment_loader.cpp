#include "Class/Manager/File_manager/Header/Equipment_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include "Class/Inheritor/Inheritor2/Permanent_Equipment/Header/Lighting.h"
#include "Class/Inheritor/Inheritor2/Single_use_Equipment/Header/Watering_Equipment.h"
#include "Class/Inheritor/Inheritor2/Single_use_Equipment/Header/Fertilizer_Equipment.h"
#include <fstream>

namespace { 
    void trim(std::string& s) {
        s.erase(0, s.find_first_not_of(" \t\r\n"));
        s.erase(s.find_last_not_of(" \t\r\n") + 1);
    }
}

void Equipment_loader::save(const std::string& path, const Collection<std::unique_ptr<Equipment>>& equipment) {
    std::ofstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "writing");

    for (const auto& eq_ptr : equipment) {
        const Equipment* eq = eq_ptr.get();
        file << "[EQUIPMENT]\n";
        file << "type=" << eq->get_type() << "\n";
        file << "name=" << eq->get_name() << "\n";
        file << "energy=" << eq->get_energy_requirement() << "\n";
        file << "broken=" << eq->get_is_broken() << "\n";
        file << "on=" << eq->get_is_on() << "\n";
        file << "wear=" << eq->get_wear_level() << "\n";
        
        if (eq->get_type() == "Watering_equipment" || eq->get_type() == "Fertilizer_equipment") {
            auto single = dynamic_cast<const Single_use_equipment*>(eq);
            file << "resource=" << single->get_resource_left() << "\n";
        }
        file << "[/EQUIPMENT]\n\n";
    }
}

Collection<std::unique_ptr<Equipment>> Equipment_loader::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "reading");

    Collection<std::unique_ptr<Equipment>> result;
    std::string line;
    std::size_t line_num = 0;

    std::string type, name;
    double energy = 0, wear = 0, resource = 100;
    bool broken = false, is_on = false;

    while (std::getline(file, line)) {
        line_num++;
        trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line == "[EQUIPMENT]") {
            type = ""; name = ""; energy = 0; wear = 0; resource = 100; broken = false; is_on = false;
        } else if (line == "[/EQUIPMENT]") {
            try {
                std::unique_ptr<Equipment> eq;
                if (type == "Lighting") eq = std::make_unique<Lighting>(name, energy, 50.0, 550.0);
                else if (type == "Watering_equipment") eq = std::make_unique<Watering_equipment>(name, energy, resource, 1.0);
                else if (type == "Fertilizer_equipment") eq = std::make_unique<Fertilizer_equipment>(name, energy, resource, 1.0);
                
                if (eq) {
                    eq->set_is_broken(broken);
                    eq->set_is_on(is_on);
                    eq->set_wear_level(wear);
                    result.push_back(std::move(eq));
                }
            } catch (const std::exception&) {
                throw File_format_exception(path, line_num, "invalid equipment data");
            }
        } else {
            auto pos = line.find('=');
            if (pos == std::string::npos) continue;
            auto key = line.substr(0, pos);
            auto val = line.substr(pos + 1);

            try {
                if (key == "type") type = val;
                else if (key == "name") name = val;
                else if (key == "energy") energy = std::stod(val);
                else if (key == "broken") broken = (val == "1");
                else if (key == "on") is_on = (val == "1");
                else if (key == "wear") wear = std::stod(val);
                else if (key == "resource") resource = std::stod(val);
            } catch (...) { throw File_format_exception(path, line_num, "invalid numeric value"); }
        }
    }
    return result;
}