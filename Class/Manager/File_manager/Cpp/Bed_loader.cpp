#include "Class/Manager/File_manager/Header/Bed_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include <fstream>

namespace {
    void trim(std::string& s) {
        s.erase(0, s.find_first_not_of(" \t\r\n"));
        s.erase(s.find_last_not_of(" \t\r\n") + 1);
    }
}

void Bed_loader::save(const std::string& path, const Collection<std::unique_ptr<Bed>>& beds) {
    std::ofstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "writing");

    for (const auto& b_ptr : beds) {
        const Bed* b = b_ptr.get();
        file << "[BED]\n";
        file << "row=" << b->get_numb_row() << "\n";
        file << "crop_row=" << b->get_numb_crop_row() << "\n";
        file << "length=" << b->get_length() << "\n";
        file << "width=" << b->get_width() << "\n";
        file << "ph=" << b->get_soil_ph() << "\n";
        file << "moisture=" << b->get_moisture() << "\n";

        for (std::size_t j = 0; j < b->get_plant_count(); ++j) {
            const Plant* p = b->get_plant(j);
            file << "  [PLANT]\n";
            file << "  id=" << p->get_id() << "\n";
            file << "  type=" << p->get_type() << "\n";
            file << "  height=" << p->get_height() << "\n";
            file << "  water_req=" << p->get_water_requirement() << "\n";
            file << "  opt_temp=" << p->get_optimal_temperature() << "\n";
            file << "  opt_hum=" << p->get_optimal_humidity() << "\n";
            file << "  stage=" << static_cast<int>(p->get_stage()) << "\n";
            file << "  stage_prog=" << p->get_stage_progress() << "\n";
            double dur = p->get_stage_duration();
            if (dur <= 0.0) dur = 100.0;
            file << "  stage_dur=" << dur << "\n";
            file << "  [/PLANT]\n";
        }
        file << "[/BED]\n\n";
    }
}

Collection<std::unique_ptr<Bed>> Bed_loader::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "reading");

    Collection<std::unique_ptr<Bed>> result;
    std::string line;
    std::size_t line_num = 0;

    std::unique_ptr<Bed> current_bed;
    
    std::string p_id, p_type;
    double p_h = 1, p_w = 1, p_t = 20, p_hu = 50, p_sp = 0, p_sd = 100;
    int p_st = 0;

    while (std::getline(file, line)) {
        line_num++;
        trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line == "[BED]") {
            current_bed = std::make_unique<Bed>();
        } else if (line == "[/BED]") {
            if (current_bed) result.push_back(std::move(current_bed));
        } else if (line == "[PLANT]") { 
            p_id = "id"; p_type = "type"; p_h = 1; p_w = 1; p_t = 20; p_hu = 50; p_sp = 0; p_sd = 100; p_st = 0; 
        } else if (line == "[/PLANT]") {
            if (current_bed) {
                auto plant = std::make_unique<Plant>(p_id, p_type, p_h, p_w, p_t, p_hu);
                plant->set_stage(static_cast<Growth_stage>(p_st));
                plant->set_stage_progress(p_sp);
                if (p_sd <= 0) p_sd = 100.0;
                plant->set_stage_duration(p_sd);
                current_bed->add_plant(std::move(plant));
            }
        } else {
            auto pos = line.find('=');
            if (pos == std::string::npos) continue;
            auto key = line.substr(0, pos);
            auto val = line.substr(pos + 1);

            try {
                if (current_bed) {
                    if (key == "row") current_bed->set_numb_row(std::stoi(val));
                    else if (key == "crop_row") current_bed->set_numb_crop_row(std::stoi(val));
                    else if (key == "length") current_bed->set_length(std::stod(val));
                    else if (key == "width") current_bed->set_width(std::stod(val));
                    else if (key == "ph") current_bed->set_soil_ph(std::stod(val));
                    else if (key == "moisture") current_bed->set_moisture(std::stod(val));
                    else if (key == "id") p_id = val;
                    else if (key == "type") p_type = val;
                    else if (key == "height") p_h = std::stod(val);
                    else if (key == "water_req") p_w = std::stod(val);
                    else if (key == "opt_temp") p_t = std::stod(val);
                    else if (key == "opt_hum") p_hu = std::stod(val);
                    else if (key == "stage") p_st = std::stoi(val);
                    else if (key == "stage_prog") p_sp = std::stod(val);
                    else if (key == "stage_dur") p_sd = std::stod(val);
                }
            } catch (...) { throw File_format_exception(path, line_num, "invalid value for key " + key); }
        }
    }
    return result;
}