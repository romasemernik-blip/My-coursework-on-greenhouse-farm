#pragma once
#include <vector>
#include"Class/Base/Header/Growth_stage.h"

class Plant
{
    protected:
    double height;
    double water_requirement;
    double current_health;
    std::unique_ptr<Growth_stage> stage;
    

};