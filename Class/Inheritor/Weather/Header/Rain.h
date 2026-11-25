#pragma once
#include "Class/Base/Header/Weather.h"

class Rain : public Weather {
public:
    Rain() : Weather("Дождь", 1.5, -0.01, 1.2) {}
};