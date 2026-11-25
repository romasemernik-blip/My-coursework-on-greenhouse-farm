#pragma once
#include <string>
#include "Class/Base/Header/Growth_stage.h"

class Plant {
protected:
    std::string id_;
    std::string type_;
    double height_;
    double water_requirement_;
    double current_health_;
    double optimal_temperature_;
    double optimal_humidity_;
    Growth_stage stage_;
    double stage_progress_;
    double stage_duration_hours_;

public:
    Plant();
    Plant(const std::string& id,
          const std::string& type,
          double height,
          double water_requirement,
          double optimal_temperature,
          double optimal_humidity);
    Plant(const Plant& other) = default;
    Plant(Plant&& other) noexcept = default;
    Plant& operator=(const Plant& other) = default;
    Plant& operator=(Plant&& other) noexcept = default;
    ~Plant() = default;

    void grow(double dt);
    void damage(double amount);
    void heal(double amount);
    void advance_stage(double dt);
    void kill();

    const std::string& get_id()                  const noexcept;
    const std::string& get_type()                const noexcept;
    double             get_height()              const noexcept;
    double             get_water_requirement()   const noexcept;
    double             get_current_health()      const noexcept;
    double             get_optimal_temperature() const noexcept;
    double             get_hours_until_mature()  const noexcept;
    double             get_optimal_humidity()    const noexcept;
    Growth_stage       get_stage()               const noexcept;
    double             get_stage_progress()      const noexcept;
    double             get_stage_duration()      const noexcept;
    bool               get_is_dead()             const noexcept;
    bool               get_is_mature()           const noexcept;
    

    void set_id(const std::string& value);
    void set_type(const std::string& value);
    void set_height(double value);
    void set_water_requirement(double value);
    void set_current_health(double value);
    void set_optimal_temperature(double value);
    void set_optimal_humidity(double value);
    void set_stage(Growth_stage value);
    void set_stage_progress(double value);
    void set_stage_duration(double value);
};