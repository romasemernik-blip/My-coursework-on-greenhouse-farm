#include "Class/Manager/File_manager/Header/Plant_type_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include <fstream>
#include <sstream>

std::vector<Plant_type_data> Plant_type_loader::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw File_open_exception(path, "reading");

    std::vector<Plant_type_data> result;
    std::string line;
    std::size_t line_num = 0;

    while (std::getline(file, line)) {
        line_num++;
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string name, temp_str, hum_str, water_str, speed_str;

        if (!std::getline(ss, name, ';') ||
            !std::getline(ss, temp_str, ';') ||
            !std::getline(ss, hum_str, ';') ||
            !std::getline(ss, water_str, ';') ||
            !std::getline(ss, speed_str, ';')) {
            throw File_format_exception(path, line_num, "invalid column count");
        }

        try {
            double temp  = std::stod(temp_str);
            double hum   = std::stod(hum_str);
            double water = std::stod(water_str);
            double speed = std::stod(speed_str);
            result.emplace_back(name, temp, hum, water, speed);
        } catch (const std::exception&) {
            throw File_format_exception(path, line_num, "invalid numeric value");
        }
    }
    return result;
}

void Plant_type_loader::save(const std::string& path,
                             const std::vector<Plant_type_data>& types) {
    std::ofstream file(path);
    if (!file.is_open())
        throw File_open_exception(path, "writing");

    for (const auto& t : types) {
        file << t.get_type_name() << ";"
             << t.get_optimal_temperature() << ";"
             << t.get_optimal_humidity() << ";"
             << t.get_water_requirement() << ";"
             << t.get_growth_speed() << "\n";
    }
}