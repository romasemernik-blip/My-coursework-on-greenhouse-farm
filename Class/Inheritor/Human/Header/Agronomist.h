#pragma once
#include"Class/Base/Header/Human.h"
#include"Class/Template/Collection.h"
#include<Class/Base/Header/Bed.h>

class Argonomist: public Human {\
    private:
    int start_year;
    int experience_year;
    Collection<unique_ptr<Bed>> assign_bed;
    public:


};