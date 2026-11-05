#pragma once
#include <vector>
#include "Class/Base/Header/Operation.h"
#include "Class/Template/Collection.h"
#include "Class/Manager/Time_manager/Header/Time_manager.h"

class Schedule
{
    private:
    bool is_paused;
    Collection<std::unique_ptr<Operation>> auto_operation;
    Time_manager* time_manager;
    
    

};