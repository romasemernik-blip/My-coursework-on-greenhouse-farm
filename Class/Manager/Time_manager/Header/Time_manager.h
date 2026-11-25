#pragma once
#include <string>

class Time_manager {
private:
    // Абсолютное игровое время в часах (например, 26.5 = 1 день, 2 часа 30 минут)
    double total_hours_; 
    double speed_multiplier_;
    bool is_paused_;

public:
    Time_manager();
    
    // Обновление времени (вызывается из таймера в MainWindow)
    void tick(double real_seconds);
    
    // Управление скоростью
    void set_speed_multiplier(double speed);
    double get_speed_multiplier() const noexcept;
    
    void set_paused(bool paused);
    bool get_is_paused() const noexcept;

    // Получение текущего времени
    double get_total_hours() const noexcept;
    int get_current_day() const noexcept;
    int get_current_hour() const noexcept;
    int get_current_minute() const noexcept;

    // Форматированная строка времени (например, "День 1 | 12:30")
    std::string get_formatted_time() const;
};