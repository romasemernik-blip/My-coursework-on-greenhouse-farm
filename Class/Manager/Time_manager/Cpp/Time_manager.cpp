#include "Class/Manager/Time_manager/Header/Time_manager.h"
#include <cmath>
#include <sstream>
#include <iomanip>

Time_manager::Time_manager() 
    : total_hours_(0.0), speed_multiplier_(1.0), is_paused_(false) {}

void Time_manager::tick(double real_seconds) {
    if (is_paused_) return;
    
    // Допустим, 1 реальная секунда = 0.1 игрового часа (6 минут)
    // Умножаем на множитель скорости (x1, x5, x10)
    double game_hours_passed = real_seconds * 0.1 * speed_multiplier_;
    total_hours_ += game_hours_passed;
}

void Time_manager::set_speed_multiplier(double speed) {
    speed_multiplier_ = speed;
}

double Time_manager::get_speed_multiplier() const noexcept {
    return speed_multiplier_;
}

void Time_manager::set_paused(bool paused) {
    is_paused_ = paused;
}

bool Time_manager::get_is_paused() const noexcept {
    return is_paused_;
}

double Time_manager::get_total_hours() const noexcept {
    return total_hours_;
}

int Time_manager::get_current_day() const noexcept {
    return static_cast<int>(total_hours_ / 24.0) + 1; // Дни начинаются с 1
}

int Time_manager::get_current_hour() const noexcept {
    return static_cast<int>(std::fmod(total_hours_, 24.0));
}

int Time_manager::get_current_minute() const noexcept {
    double fractional_hour = std::fmod(total_hours_, 1.0);
    return static_cast<int>(fractional_hour * 60.0);
}

void Time_manager::set_total_hours(double hours) {
    total_hours_ = hours;
}

std::string Time_manager::get_formatted_time() const {
    std::ostringstream os;
    os << "День " << get_current_day() 
       << " | " << std::setfill('0') << std::setw(2) << get_current_hour()
       << ":" << std::setfill('0') << std::setw(2) << get_current_minute();
    return os.str();
}