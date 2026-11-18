#pragma once
#include <string>

enum class Growth_stage {
    Seed,
    Vegetative,
    Flowering,
    Fruiting,
    Dead
};

inline std::string growth_stage_to_string(Growth_stage stage) {
    switch (stage) {
    case Growth_stage::Seed:       return "Seed";
    case Growth_stage::Vegetative: return "Vegetative";
    case Growth_stage::Flowering:  return "Flowering";
    case Growth_stage::Fruiting:   return "Fruiting";
    case Growth_stage::Dead:       return "Dead";
    }
    return "Unknown";
}

inline bool growth_stage_is_mature(Growth_stage stage) {
    return stage == Growth_stage::Fruiting;
}

inline bool growth_stage_is_dead(Growth_stage stage) {
    return stage == Growth_stage::Dead;
}