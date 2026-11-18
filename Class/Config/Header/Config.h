#pragma once
#include <cstddef>

namespace Config {
    // Растения
    constexpr double DEFAULT_PLANT_HEIGHT = 0.0;
    constexpr double DEFAULT_WATER_REQUIREMENT = 0.0;
    constexpr double DEFAULT_PLANT_HEALTH = 100.0;
    constexpr double MIN_PLANT_HEALTH = 0.0;
    constexpr double MAX_PLANT_HEALTH = 100.0;
    
    constexpr double DEFAULT_OPTIMAL_TEMPERATURE = 22.0;
    constexpr double DEFAULT_OPTIMAL_HUMIDITY = 60.0;
    constexpr double MIN_HUMIDITY = 0.0;
    constexpr double MAX_HUMIDITY = 100.0;
    constexpr double DEFAULT_GROWTH_SPEED = 1.0;

    // Стадии роста (часы)
    constexpr double STAGE_SEED_DURATION = 24.0;
    constexpr double STAGE_VEGETATIVE_DURATION = 48.0;
    constexpr double STAGE_FLOWERING_DURATION = 36.0;
    constexpr double STAGE_FRUITING_DURATION = 72.0;
    constexpr double MIN_STAGE_DURATION = 0.0;
    constexpr double MAX_STAGE_PROGRESS = 1.0;

    // Оборудование
    constexpr double DEFAULT_ENERGY_REQUIREMENT = 0.0;
    constexpr double DEFAULT_WEAR_LEVEL = 0.0;
    constexpr double MIN_WEAR_LEVEL = 0.0;
    constexpr double MAX_WEAR_LEVEL = 100.0;
    
    constexpr double DEFAULT_BRIGHTNESS = 50.0;
    constexpr double DEFAULT_SPECTRUM_NM = 550.0;
    constexpr double MIN_SPECTRUM_NM = 380.0;
    constexpr double MAX_SPECTRUM_NM = 780.0;
    
    constexpr double DEFAULT_RESOURCE_LEFT = 0.0;
    constexpr double DEFAULT_RESOURCE_CONSUMPTION = 1.0;
    
    constexpr double DEFAULT_SENSOR_MIN = -100.0;
    constexpr double DEFAULT_SENSOR_MAX = 100.0;

    // Грядки
    constexpr double DEFAULT_SOIL_PH = 7.0;
    constexpr double MIN_SOIL_PH = 0.0;
    constexpr double MAX_SOIL_PH = 14.0;
    constexpr double DEFAULT_MOISTURE = 50.0;
    constexpr double DEFAULT_READINESS = 0.0;
    constexpr double DEFAULT_CO2_LEVEL = 400.0;

    // Теплица
    constexpr double DEFAULT_ENERGY = 1000.0;
    constexpr std::size_t DEFAULT_BED_CAPACITY = 100;

    // Время (менеджер времени)
    constexpr double DEFAULT_SPEED_MULTIPLIER = 1.0;
    constexpr double START_HOUR = 6.0;
    constexpr std::size_t START_DAY = 1;
    constexpr double SECONDS_IN_HOUR = 3600.0;
    constexpr double HOURS_IN_DAY = 24.0;
    constexpr double DAYTIME_START = 6.0;
    constexpr double DAYTIME_END = 20.0;

    // Расписание / Автоматизация
    constexpr double DEFAULT_AUTO_INTERVAL = 1.0;
    constexpr int DEFAULT_PRIORITY = 0;

    // Ввод / Логика
    constexpr double EPSILON = 0.00001;
    // Оператор
    constexpr int MIN_ACCESS_LEVEL = 0;
    constexpr int MAX_ACCESS_LEVEL = 5;
    constexpr const char* DEFAULT_SHIFT = "Day";
}