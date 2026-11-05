#pragma once
#include"Class/Inheritor/Human/Header/Agronomist.h"
#include"Class/Inheritor/Equipment/Header/Single_use_Equipment.h"

class Operation
{
    double energy_cost;
    std::unique_ptr<Human> worker;
    std::unique_ptr<Equipment> equipment;
    std::string name;

};