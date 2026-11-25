#include "Class/Manager/File_manager/Header/Storage_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include <fstream>

void Storage_loader::save_to_file(const std::string& path, const Storage& storage) {
    std::ofstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "writing");

    file << "[STORAGE_EXPORT]\n";
    file << "capacity=" << storage.get_capacity() << "\n";
    
    for (const auto& [crop, amount] : storage.get_contents()) {
        file << crop << "=" << amount << "\n";
    }
}

void Storage_loader::load_from_file(const std::string& path, Storage& storage) {
    std::ifstream file(path);
    if (!file.is_open()) throw File_open_exception(path, "reading");

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '[') continue;

        auto pos = line.find('=');
        if (pos != std::string::npos) {
            auto key = line.substr(0, pos);
            auto val = line.substr(pos + 1);
            
            if (key == "capacity") {
                storage.set_capacity(std::stod(val));
            } else {
                storage.add(key, std::stod(val));
            }
        }
    }
}