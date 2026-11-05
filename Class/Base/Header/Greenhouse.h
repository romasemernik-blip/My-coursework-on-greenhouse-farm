#pragma once
#include<string.h>
#include <vector>
#include"Class/Base/Header/Equipment.h"
#include"Class/Base/Header/Plant.h"
#include"Class/Base/Header/Bed.h"
#include"Class/Template/Collection.h"

class Greehouse
{
    private:
    std::string name;
    int number_of_beds;
    Collection<std::unique_ptr<Equipment>> equipment;
    int energy;
    Collection<std::unique_ptr<Bed>> beds;
    size_t length;
    size_t width;
    size_t height;

    public:

};