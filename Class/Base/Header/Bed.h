#pragma once
#include"Class/Template/Collection.h"
#include"Class/Base/Header/Plant.h"
#include"Class/Base/Header/Equipment.h"
#include <vector>

class Bed
{
    private:
    int numb_row;
    int numb_crop_row;
    Collection<std::unique_ptr<Plant>> plants;
    Collection<std::unique_ptr<Equipment>> equipments;
    double soil_ph;
    double moisture;
    double readiness;
    double co2_level;
    
    public:


    
    



};