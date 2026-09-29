#pragma once
#include<string.h>
#include <vector>
#include"Class/Base/Header/Equipment.h"
#include"Class/Base/Header/Plant.h"
#include"Class/Base/Header/Bed.h"
class Greehouse
{
    private:
    std::string name;
    int number_of_beds;
    std::vector<std::unique_ptr<Equipment>> equipment;
    int energy;
    std::vector<std::unique_ptr<Bed>> beds;
    public:

};