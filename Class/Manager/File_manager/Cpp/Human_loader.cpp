#include "Class/Manager/File_manager/Header/Human_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include "Class/Inheritor/Human/Header/Agronomist.h"
#include "Class/Inheritor/Human/Header/Operator.h"
#include <fstream>

void Human_loader::save(const std::string& path, const Collection<std::unique_ptr<Human>>& humans) {
    std::ofstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "writing");

    for (std::size_t i = 0; i < humans.size(); ++i) {
        const Human* h = humans[i].get();
        file << "[HUMAN]\n";
        file << "role=" << h->get_role() << "\n";
        file << "first=" << h->get_first_name() << "\n";
        file << "last=" << h->get_last_name() << "\n";
        file << "middle=" << h->get_middle_name() << "\n";
        file << "birth=" << h->get_birth_date() << "\n";
        file << "gender=" << h->get_gender() << "\n";

        if (h->get_role() == "Agronomist") {
            const Agronomist* agr = dynamic_cast<const Agronomist*>(h);
            file << "start_year=" << agr->get_start_year() << "\n";
            file << "exp=" << agr->get_experience_years() << "\n";
        } else if (h->get_role() == "Operator") {
            const Operator* op = dynamic_cast<const Operator*>(h);
            file << "shift=" << op->get_shift() << "\n";
            file << "access=" << op->get_access_level() << "\n";
        }
        file << "[/HUMAN]\n\n";
    }
}

Collection<std::unique_ptr<Human>> Human_loader::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "reading");

    Collection<std::unique_ptr<Human>> result;
    std::string line;
    std::size_t line_num = 0;

    std::string role="", fname="", lname="", mname="", gender="", shift="";
    std::size_t birth=0; int f1=0, f2=0;

    while (std::getline(file, line)) {
        line_num++;
        if (line.empty() || line[0] == '#') continue;

        if (line == "[HUMAN]") {
            role=""; fname=""; lname=""; mname=""; gender=""; shift=""; birth=0; f1=0; f2=0;
        } else if (line == "[/HUMAN]") {
            try {
                if (role == "Agronomist")
                    result.push_back(std::make_unique<Agronomist>(fname, lname, mname, birth, gender, f1, f2));
                else if (role == "Operator")
                    result.push_back(std::make_unique<Operator>(fname, lname, mname, birth, gender, shift, f1));
            } catch (const std::exception& e) {
                throw File_format_exception(path, line_num, std::string("Error creating human: ") + e.what());
            }
        } else {
            auto pos = line.find('=');
            if (pos == std::string::npos) continue;
            std::string key = line.substr(0, pos), val = line.substr(pos + 1);

            try {
                if (key == "role") role = val;
                else if (key == "first") fname = val;
                else if (key == "last") lname = val;
                else if (key == "middle") mname = val;
                else if (key == "birth") birth = std::stoul(val);
                else if (key == "gender") gender = val;
                else if (key == "start_year" || key == "access") f1 = std::stoi(val);
                else if (key == "exp") f2 = std::stoi(val);
                else if (key == "shift") shift = val;
            } catch (...) { throw File_format_exception(path, line_num, "invalid value"); }
        }
    }
    return result;
}