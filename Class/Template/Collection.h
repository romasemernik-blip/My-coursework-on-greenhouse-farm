#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <cstddef>
#include <utility>
#include "Class/Exception/Header/All_exception.h"

template <typename T>
class Collection {
private:
    std::vector<T> items_;

public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    Collection() = default; 
    Collection(const Collection& other) = default;
    Collection(Collection&& other) noexcept = default;
    Collection& operator=(const Collection& other) = default;
    Collection& operator=(Collection&& other) noexcept = default;
    ~Collection() = default;

    iterator begin() { return items_.begin(); }
    iterator end()   { return items_.end(); }
    const_iterator begin() const { return items_.begin(); }
    const_iterator end()   const { return items_.end(); }

    T& operator[](std::size_t index) {
        if (index >= items_.size()) throw Invalid_argument_exception("index", "out of range");
        return items_[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= items_.size()) throw Invalid_argument_exception("index", "out of range");
        return items_[index];
    }

    T& get(std::size_t index) {
        if (index >= items_.size()) throw Invalid_argument_exception("index", "out of range");
        return items_[index];
    }

    const T& get(std::size_t index) const {
        if (index >= items_.size()) throw Invalid_argument_exception("index", "out of range");
        return items_[index];
    }

    void push_back(const T& item) { items_.push_back(item); }
    void push_back(T&& item) { items_.push_back(std::move(item)); }
    bool empty() const noexcept { return items_.empty(); }
    std::size_t size() const noexcept { return items_.size(); }
    void clear() noexcept { items_.clear(); }

    void remove_at(std::size_t index) {
        if (index >= items_.size()) throw Invalid_argument_exception("index", "out of range");
        items_.erase(items_.begin() + index);
    }

    template <typename Pred>
    int find_if(Pred pred) const {
        for (std::size_t i = 0; i < items_.size(); ++i) {
            if (pred(items_[i])) return static_cast<int>(i);
        }
        return -1;
    }

    template <typename Pred>
    Collection<T*> filter(Pred pred) {
        Collection<T*> result;
        for (auto& item : items_) {
            if (pred(item)) result.push_back(&item);
        }
        return result;
    }

    template <typename Pred>
    Collection<const T*> filter(Pred pred) const {
        Collection<const T*> result;
        for (const auto& item : items_) {
            if (pred(item)) result.push_back(&item);
        }
        return result;
    }

    template <typename Key>
    void sort_by(Key key) {
        std::sort(items_.begin(), items_.end(),
                  [&](const T& a, const T& b) { return key(a) < key(b); });
    }

    template <typename Init, typename BinOp>
    Init accumulate(Init init, BinOp op) const {
        for (const auto& item : items_) {
            init = op(init, item);
        }
        return init;
    }
};