#include "Class/Manager/File_manager/Header/Progress_saver.h"
#include "Class/Exception/Header/All_exception.h"
#include <fstream>
#include <sstream>

void Progress_saver::save(const std::string& path,
                          const Greenhouse& greenhouse,
                          const Storage& storage,
                          const Time_manager& time_manager) {
    std::ofstream file(path);
    if (!file.is_open())
        throw File_open_exception(path, "writing");

    file << "[TIME]\n";
    file << "hour=" << time_manager.get_current_hour() << "\n";
    file << "day=" << time_manager.get_current_day() << "\n";
    file << "speed=" << time_manager.get_speed_multiplier() << "\n";

    file << "[GREENHOUSE]\n";
    file << "name=" << greenhouse.get_name() << "\n";
    file << "length=" << greenhouse.get_length() << "\n";
    file << "width=" << greenhouse.get_width() << "\n";
    file << "height=" << greenhouse.get_height() << "\n";
    file << "max_energy=" << greenhouse.get_max_energy() << "\n";
    file << "current_energy=" << greenhouse.get_current_energy() << "\n";

    file << "[STORAGE]\n";
    file << "capacity=" << storage.get_capacity() << "\n";
    file << "load=" << storage.get_current_load() << "\n";
}

void Progress_saver::load(const std::string& path,
                          Greenhouse& greenhouse,
                          Storage& storage,
                          Time_manager& time_manager) {
    std::ifstream file(path);
    if (!file.is_open())
        throw File_open_exception(path, "reading");

    std::string line;
    std::size_t line_num = 0;

    while (std::getline(file, line)) {
        line_num++;
        if (line.empty() || line[0] == '[') continue;

        auto pos = line.find('=');
        if (pos == std::string::npos)
            throw File_format_exception(path, line_num, "missing '=' separator");

        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);

        try {
            if (key == "hour")            time_manager.set_current_hour(std::stod(val));
            else if (key == "day")        time_manager.set_current_day(std::stoul(val));
            else if (key == "speed")      time_manager.set_speed_multiplier(std::stod(val));
            else if (key == "max_energy") greenhouse.set_max_energy(std::stod(val));
            else if (key == "current_energy") greenhouse.set_current_energy(std::stod(val));
            else if (key == "capacity")   storage.set_capacity(std::stod(val));
        } catch (const std::exception&) {
            throw File_format_exception(path, line_num, "invalid numeric value for key " + key);
        }
    }
}