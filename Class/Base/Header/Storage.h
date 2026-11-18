#pragma once
#include <string>
#include <map>

class Storage {
private:
    double capacity_;
    double current_load_;
    std::map<std::string, double> contents_;

public:
    Storage();
    Storage(double capacity);
    Storage(const Storage& other) = default;
    Storage(Storage&& other) noexcept = default;
    Storage& operator=(const Storage& other) = default;
    Storage& operator=(Storage&& other) noexcept = default;
    ~Storage() = default;

    void add(const std::string& crop, double amount);
    void remove(const std::string& crop, double amount);
    double get_amount(const std::string& crop) const;
    void clear();

    double get_capacity()     const noexcept;
    double get_current_load() const noexcept;
    double get_free_space()   const noexcept;

    void set_capacity(double value);
    const std::map<std::string, double>& get_contents() const noexcept { return contents_; }
};