#include "Class/Logger/Header/System_logger.h"
#include <QDateTime>

System_logger::System_logger(const std::string& filepath) {
    // Открываем в режиме добавления (app)
    file_.open(filepath, std::ios::app);
}

System_logger::~System_logger() {
    if (file_.is_open()) file_.close();
}

void System_logger::log(const std::string& message) {
    if (file_.is_open()) {
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        file_ << "[" << time.toStdString() << "] " << message << "\n";
        file_.flush();
    }
}