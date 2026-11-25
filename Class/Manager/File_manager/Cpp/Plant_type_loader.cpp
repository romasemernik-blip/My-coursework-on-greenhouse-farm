#include "Class/Manager/File_manager/Header/Plant_type_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include <fstream>

std::vector<Plant_type_data> Plant_type_loader::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "reading");

    std::vector<Plant_type_data> result;
    std::string line;
    std::size_t line_num = 0;
    
    std::string name=""; double temp=0, hum=0, water=0, speed=0;

    while (std::getline(file, line)) {
        line_num++;
        if (line.empty() || line[0] == '#') continue;

        if (line == "[TYPE]") {
            name=""; temp=0; hum=0; water=0; speed=0;
        } else if (line == "[/TYPE]") {
            try {
                result.emplace_back(name, temp, hum, water, speed);
            } catch (const std::exception& e) {
                throw File_format_exception(path, line_num, std::string("Error creating type: ") + e.what());
            }
        } else {
            auto pos = line.find('=');
            if (pos == std::string::npos) continue;
            std::string key = line.substr(0, pos), val = line.substr(pos + 1);

            try {
                if (key == "name") name = val;
                else if (key == "temp") temp = std::stod(val);
                else if (key == "hum") hum = std::stod(val);
                else if (key == "water") water = std::stod(val);
                else if (key == "speed") speed = std::stod(val);
            } catch (...) { throw File_format_exception(path, line_num, "invalid numeric value"); }
        }
    }
    return result;
}

void Plant_type_loader::save(const std::string& path, const std::vector<Plant_type_data>& types) {
    std::ofstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "writing");

    for (const auto& t : types) {
        file << "[TYPE]\n";
        file << "name=" << t.get_type_name() << "\n";
        file << "temp=" << t.get_optimal_temperature() << "\n";
        file << "hum=" << t.get_optimal_humidity() << "\n";
        file << "water=" << t.get_water_requirement() << "\n";
        file << "speed=" << t.get_growth_speed() << "\n";
        file << "[/TYPE]\n\n";
    }
}