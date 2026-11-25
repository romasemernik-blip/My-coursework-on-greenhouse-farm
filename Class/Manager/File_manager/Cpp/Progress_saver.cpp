#include "Class/Manager/File_manager/Header/Progress_saver.h"
#include "Class/Exception/Header/All_exception.h"
#include <fstream>
#include <sstream>

void Progress_saver::save(const std::string& path, const Greenhouse& greenhouse, const Storage& storage, const Time_manager& time_manager) {
    std::ofstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "writing");

    file << "[TIME]\n";
    file << "total_hours=" << time_manager.get_total_hours() << "\n";
    file << "speed=" << time_manager.get_speed_multiplier() << "\n\n";

    file << "[GREENHOUSE]\n";
    file << "name=" << greenhouse.get_name() << "\n";
    file << "length=" << greenhouse.get_length() << "\n";
    file << "width=" << greenhouse.get_width() << "\n";
    file << "height=" << greenhouse.get_height() << "\n";
    file << "max_energy=" << greenhouse.get_max_energy() << "\n";
    file << "current_energy=" << greenhouse.get_current_energy() << "\n\n";

    file << "[STORAGE]\n";
    file << "capacity=" << storage.get_capacity() << "\n";
    for (const auto& [crop, amount] : storage.get_contents()) { 
        file << "item_" << crop << "=" << amount << "\n";
    }
}

void Progress_saver::load(const std::string& path, Greenhouse& greenhouse, Storage& storage, Time_manager& time_manager) {
    std::ifstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "reading");

    std::string line;
    std::size_t line_num = 0;

    while (std::getline(file, line)) {
        line_num++;
        if (line.empty() || line[0] == '[' || line[0] == '#') continue;

        auto pos = line.find('=');
        if (pos == std::string::npos) continue;

        auto key = line.substr(0, pos);
        auto val = line.substr(pos + 1);

        try {
            if (key == "total_hours") time_manager.set_total_hours(std::stod(val));
            else if (key == "speed") time_manager.set_speed_multiplier(std::stod(val));
            else if (key == "name") greenhouse.set_name(val);
            else if (key == "length") greenhouse.set_length(std::stod(val));
            else if (key == "width") greenhouse.set_width(std::stod(val));
            else if (key == "height") greenhouse.set_height(std::stod(val));
            else if (key == "max_energy") greenhouse.set_max_energy(std::stod(val));
            else if (key == "current_energy") greenhouse.set_current_energy(std::stod(val));
            else if (key == "capacity") storage.set_capacity(std::stod(val));
            else if (key.rfind("item_", 0) == 0) {
                storage.add(key.substr(5), std::stod(val));
            }
        } catch (const std::exception&) {
            throw File_format_exception(path, line_num, "invalid value for key " + key);
        }
    }
}