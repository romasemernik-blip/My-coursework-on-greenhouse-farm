#pragma once
#include "Class/Base/Header/Weather.h"

class Snow : public Weather {
public:
    Snow() : Weather("Снег", 0.5, 0.0, 0.1) {} // Немного влаги, рост почти остановлен
};