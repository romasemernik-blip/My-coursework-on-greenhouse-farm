#include "Class/Manager/Time_manager/Header/Time_manager.h"
#include <cmath>
#include <sstream>
#include <iomanip>

Time_manager::Time_manager() 
    : total_hours_(0.0), speed_multiplier_(1.0), is_paused_(false) {}

void Time_manager::tick(double real_seconds) {
    if (is_paused_) return;
    // 1 реальная секунда = 0.1 игрового часа
    total_hours_ += real_seconds * 0.1 * speed_multiplier_;
}

void Time_manager::set_speed_multiplier(double speed) { speed_multiplier_ = speed; }
double Time_manager::get_speed_multiplier() const noexcept { return speed_multiplier_; }

void Time_manager::set_paused(bool paused) { is_paused_ = paused; }
bool Time_manager::get_is_paused() const noexcept { return is_paused_; }

double Time_manager::get_total_hours() const noexcept { return total_hours_; }

int Time_manager::get_current_day() const noexcept {
    return static_cast<int>(total_hours_ / 24.0) + 1;
}

int Time_manager::get_current_hour() const noexcept {
    return static_cast<int>(std::fmod(total_hours_, 24.0));
}

int Time_manager::get_current_minute() const noexcept {
    return static_cast<int>(std::fmod(total_hours_, 1.0) * 60.0);
}

std::string Time_manager::get_formatted_time() const {
    std::ostringstream os;
    os << "День " << get_current_day() 
       << " | " << std::setfill('0') << std::setw(2) << get_current_hour()
       << ":" << std::setfill('0') << std::setw(2) << get_current_minute();
    return os.str();
}