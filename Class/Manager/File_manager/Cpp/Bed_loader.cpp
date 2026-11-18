#include "Class/Manager/File_manager/Header/Bed_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include <fstream>
#include <sstream>

void Bed_loader::save(const std::string& path,
                      const Collection<std::unique_ptr<Bed>>& beds) {
    std::ofstream file(path);
    if (!file.is_open())
        throw File_open_exception(path, "writing");

    for (std::size_t i = 0; i < beds.size(); ++i) {
        const Bed* b = beds[i].get();
        file << b->get_numb_row() << ";"
             << b->get_numb_crop_row() << ";"
             << b->get_length() << ";"
             << b->get_width() << ";"
             << b->get_soil_ph() << ";"
             << b->get_moisture() << ";"
             << b->get_readiness() << ";"
             << b->get_co2_level() << "\n";
    }
}

Collection<std::unique_ptr<Bed>> Bed_loader::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw File_open_exception(path, "reading");

    Collection<std::unique_ptr<Bed>> result;
    std::string line;
    std::size_t line_num = 0;

    while (std::getline(file, line)) {
        line_num++;
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string r_str, cr_str, len_str, w_str, ph_str, mo_str, re_str, co2_str;

        if (!std::getline(ss, r_str, ';') ||
            !std::getline(ss, cr_str, ';') ||
            !std::getline(ss, len_str, ';') ||
            !std::getline(ss, w_str, ';') ||
            !std::getline(ss, ph_str, ';') ||
            !std::getline(ss, mo_str, ';') ||
            !std::getline(ss, re_str, ';') ||
            !std::getline(ss, co2_str, ';')) {
            throw File_format_exception(path, line_num, "invalid bed column count");
        }

        try {
            auto bed = std::make_unique<Bed>(
                std::stoi(r_str), std::stoi(cr_str),
                std::stod(len_str), std::stod(w_str));
            bed->set_soil_ph(std::stod(ph_str));
            bed->set_moisture(std::stod(mo_str));
            bed->set_readiness(std::stod(re_str));
            bed->set_co2_level(std::stod(co2_str));
            result.push_back(std::move(bed));
        } catch (const std::exception&) {
            throw File_format_exception(path, line_num, "invalid bed data");
        }
    }
    return result;
}