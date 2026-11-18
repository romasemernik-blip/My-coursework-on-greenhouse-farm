#pragma once
#include <cstddef>

class Time_manager {
private:
    double real_time_seconds_;
    double current_hour_;
    std::size_t current_day_;
    double speed_multiplier_;
    bool is_paused_;

public:
    Time_manager();
    Time_manager(double speed_multiplier);
    Time_manager(const Time_manager& other) = default;
    Time_manager(Time_manager&& other) noexcept = default;
    Time_manager& operator=(const Time_manager& other) = default;
    Time_manager& operator=(Time_manager&& other) noexcept = default;
    ~Time_manager() = default;

    void tick(double real_seconds);
    void reset();

    void pause();
    void resume();
    void toggle_pause();

    double get_real_time_seconds() const noexcept;
    double get_current_hour()      const noexcept;
    std::size_t get_current_day()  const noexcept;
    double get_speed_multiplier()  const noexcept;
    bool   get_is_paused()         const noexcept;
    bool   is_daytime()            const noexcept;

    void set_speed_multiplier(double value);
    void set_current_hour(double value);
    void set_current_day(std::size_t value);
};