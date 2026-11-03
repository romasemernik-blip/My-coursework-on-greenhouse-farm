#pragma once
#include <vector>
#include <iostream>

template <typename T>
class Collection {
    private:
    std::vector<T> items;

    public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    const_iterator begin() {
        return items.begin();
    }
    const_iterator end() {
        return items.end();
    }

    iterator begin() {
        return items.begin();
    }
    iterator end() {
        return items.end();
    }

    T& operator[] (size_t index) { 
        return items[index];
    }
    const T& operator[] (size_t index) const { 
        return items[index];
    }

    void push_back(const T& item) {
        items.push_back(item);
    }
     void push_back(T&& item) {
        items.push_back(std::move(item));
    }

    bool empty() const {
        return items.empty();
    }

    bool remove_at(size_t index) {
        if (index >=items.size()) return false;
        items.erase(items.begin() + index);
        return true;
    }
    
    T& get(size_t index) {
        return items[index];
    }
    const T& get(size_t index) const {
        return items[index];
    }

    int find( const T& item) const {
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i] == item) return static_cast<int>(i);
        }
        return -1;
    }

    size_t size() const {
        return items.size();
    }
    void clear() {
        items.clear();
    }
    
};