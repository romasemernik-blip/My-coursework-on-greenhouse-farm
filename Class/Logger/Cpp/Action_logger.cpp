#include "Class/Logger/Header/Action_logger.h"
#include <QDateTime>

Action_logger::Action_logger(const std::string& filepath) {
    file_.open(filepath, std::ios::app);
}

Action_logger::~Action_logger() {
    try {
        if (file_.is_open()) file_.close();
    } catch (...) {
    }
}

void Action_logger::log_action(const std::string& role, const std::string& name, const std::string& action) {
    if (file_.is_open()) {
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        file_ << "[" << time.toStdString() << "] "
              << "[" << role << ": " << name << "] "
              << action << "\n";
        file_.flush();
    }
}