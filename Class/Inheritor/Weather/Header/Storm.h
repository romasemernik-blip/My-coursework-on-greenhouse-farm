#pragma once
#include "Class/Base/Header/Weather.h"

class Storm : public Weather {
public:
    Storm() : Weather("Гроза", 3.0, -0.05, 0.5) {} // Сильно поливает, кислит, стресс для роста
};