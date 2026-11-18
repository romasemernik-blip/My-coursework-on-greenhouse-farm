#include "Class/Manager/File_manager/Header/Human_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include "Class/Inheritor/Human/Header/Agronomist.h"
#include "Class/Inheritor/Human/Header/Operator.h"
#include "Class/Config/Header/Config.h"
#include <fstream>
#include <sstream>

void Human_loader::save(const std::string& path,
                        const Collection<std::unique_ptr<Human>>& humans) {
    std::ofstream file(path);
    if (!file.is_open())
        throw File_open_exception(path, "writing");

    for (std::size_t i = 0; i < humans.size(); ++i) {
        const Human* h = humans[i].get();
        file << h->get_role() << ";"
             << h->get_first_name() << ";"
             << h->get_last_name() << ";"
             << h->get_middle_name() << ";"
             << h->get_birth_date() << ";"
             << h->get_gender();

        if (h->get_role() == "Agronomist") {
            const Agronomist* agr = dynamic_cast<const Agronomist*>(h);
            file << ";" << agr->get_start_year()
                 << ";" << agr->get_experience_years();
        } else if (h->get_role() == "Operator") {
            const Operator* op = dynamic_cast<const Operator*>(h);
            file << ";" << op->get_shift()
                 << ";" << op->get_access_level();
        }
        file << "\n";
    }
}

Collection<std::unique_ptr<Human>> Human_loader::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw File_open_exception(path, "reading");

    Collection<std::unique_ptr<Human>> result;
    std::string line;
    std::size_t line_num = 0;

    while (std::getline(file, line)) {
        line_num++;
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string role, fname, lname, mname, birth, gender, field1, field2;

        if (!std::getline(ss, role, ';') ||
            !std::getline(ss, fname, ';') ||
            !std::getline(ss, lname, ';') ||
            !std::getline(ss, mname, ';') ||
            !std::getline(ss, birth, ';') ||
            !std::getline(ss, gender, ';')) {
            throw File_format_exception(path, line_num, "invalid human column count");
        }

        try {
            std::size_t birth_year = std::stoul(birth);
            
            if (role == "Agronomist") {
                if (!std::getline(ss, field1, ';') || !std::getline(ss, field2, ';'))
                    throw File_format_exception(path, line_num, "missing agronomist fields");
                result.push_back(std::make_unique<Agronomist>(
                    fname, lname, mname, birth_year, gender,
                    std::stoi(field1), std::stoi(field2)));
            } 
            else if (role == "Operator") {
                if (!std::getline(ss, field1, ';') || !std::getline(ss, field2, ';'))
                    throw File_format_exception(path, line_num, "missing operator fields");
                result.push_back(std::make_unique<Operator>(
                    fname, lname, mname, birth_year, gender,
                    field1, std::stoi(field2)));
            } 
            else {
                throw File_format_exception(path, line_num, "unknown human role: " + role);
            }
        } catch (const File_format_exception&) {
            throw;
        } catch (const std::exception&) {
            throw File_format_exception(path, line_num, "invalid human data");
        }
    }
    return result;
}