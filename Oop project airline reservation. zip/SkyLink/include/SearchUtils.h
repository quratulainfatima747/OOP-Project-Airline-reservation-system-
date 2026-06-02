#ifndef SEARCH_UTILS_H
#define SEARCH_UTILS_H

#include <vector>
#include <functional>
#include <algorithm>

// Generic linear search utility – works on any container of pointers
// Predicate: bool(const T*)
template <typename T>
std::vector<T*> searchAll(const std::vector<T*>& collection,
                          std::function<bool(const T*)> predicate) {
    std::vector<T*> results;
    for (T* item : collection) {
        if (item && predicate(item)) {
            results.push_back(item);
        }
    }
    return results;
}

// Find first match; returns nullptr if not found
template <typename T>
T* findFirst(const std::vector<T*>& collection,
             std::function<bool(const T*)> predicate) {
    auto it = std::find_if(collection.begin(), collection.end(),
                           [&](T* item) { return item && predicate(item); });
    return (it != collection.end()) ? *it : nullptr;
}

#endif // SEARCH_UTILS_H
