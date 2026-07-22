#pragma once

#include <vector>

template<typename T, typename ...Args>
inline std::vector<T> makeVector(Args&& ...args) {
    std::vector<T> vector;

    vector.reserve(sizeof...(Args));
    (vector.emplace_back(std::forward<Args>(args)), ...);

    return vector;
}