#pragma once
#include <memory>
#include "Class/Template/Collection.h"
#include "Class/Base/Header/Operation.h"
#include "Class/Manager/Time_manager/Header/Time_manager.h"

class Schedule {
private:
    bool is_paused_;
    Collection<std::unique_ptr<Operation>> operations_;
    Time_manager* time_manager_;

public:
    Schedule();
    Schedule(Time_manager* time_manager);
    Schedule(const Schedule& other) = delete;
    Schedule(Schedule&& other) noexcept;
    Schedule& operator=(const Schedule& other) = delete;
    Schedule& operator=(Schedule&& other) noexcept;
    ~Schedule() = default;

    void add_operation(std::unique_ptr<Operation> operation);
    std::unique_ptr<Operation> remove_operation(std::size_t index);
    void clear_operations();

    void update(double dt);
    void execute_all();

    void pause();
    void resume();

    bool get_is_paused() const noexcept;
    std::size_t get_operation_count() const noexcept;
    Operation* get_operation(std::size_t index);
    const Operation* get_operation(std::size_t index) const;
    Time_manager* get_time_manager() noexcept;
    const Time_manager* get_time_manager() const noexcept;

    void set_time_manager(Time_manager* time_manager);
};