#pragma once
#include "Class/Base/Header/Weather.h"

class Clear : public Weather {
public:
    Clear() : Weather("Ясно", -0.2, 0.0, 1.0) {}
};