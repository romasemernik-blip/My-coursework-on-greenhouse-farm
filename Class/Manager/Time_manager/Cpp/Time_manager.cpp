#include "Class/Manager/Time_manager/Header/Time_manager.h"
#include "Class/Exception/Header/All_exception.h"

Time_manager::Time_manager()
    : real_time_seconds_(0.0)
    , current_hour_(6.0)
    , current_day_(1)
    , speed_multiplier_(1.0)
    , is_paused_(false)
{}

Time_manager::Time_manager(double speed_multiplier)
    : real_time_seconds_(0.0)
    , current_hour_(6.0)
    , current_day_(1)
    , speed_multiplier_(speed_multiplier)
    , is_paused_(false)
{
    if (speed_multiplier <= 0.0)
        throw Invalid_argument_exception("speed_multiplier", "must be positive");
}

void Time_manager::tick(double real_seconds) {
    if (real_seconds < 0.0)
        throw Invalid_argument_exception("real_seconds", "must be non-negative");
    if (is_paused_) return;

    real_time_seconds_ += real_seconds;
    double game_seconds = real_seconds * speed_multiplier_;
    
    current_hour_ += game_seconds / 3600.0;

    while (current_hour_ >= 24.0) {
        current_hour_ -= 24.0;
        current_day_++;
    }
}

void Time_manager::reset() {
    real_time_seconds_ = 0.0;
    current_hour_ = 6.0;
    current_day_ = 1;
}

void Time_manager::pause() {
    is_paused_ = true;
}

void Time_manager::resume() {
    is_paused_ = false;
}

void Time_manager::toggle_pause() {
    is_paused_ = !is_paused_;
}

double Time_manager::get_real_time_seconds() const noexcept { return real_time_seconds_; }
double Time_manager::get_current_hour()      const noexcept { return current_hour_; }
std::size_t Time_manager::get_current_day()  const noexcept { return current_day_; }
double Time_manager::get_speed_multiplier()  const noexcept { return speed_multiplier_; }
bool   Time_manager::get_is_paused()         const noexcept { return is_paused_; }

bool Time_manager::is_daytime() const noexcept {
    return current_hour_ >= 6.0 && current_hour_ < 20.0;
}

void Time_manager::set_speed_multiplier(double value) {
    if (value <= 0.0)
        throw Invalid_argument_exception("speed_multiplier", "must be positive");
    speed_multiplier_ = value;
}

void Time_manager::set_current_hour(double value) {
    if (value < 0.0 || value >= 24.0)
        throw Invalid_argument_exception("current_hour", "must be in [0, 24)");
    current_hour_ = value;
}

void Time_manager::set_current_day(std::size_t value) {
    if (value == 0)
        throw Invalid_argument_exception("current_day", "must be positive");
    current_day_ = value;
}