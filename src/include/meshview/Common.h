//
// Created by Stefan on 16.03.2020.
//

#ifndef LEARNOGL_COMMON_H
#define LEARNOGL_COMMON_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
#include <iostream>
#include <sstream>
#include <string>

// Some convenience typedefs
using vec3 = std::array<float, 3>;
using vec6 = std::array<float, 6>;
using vec3ui = std::array<unsigned int, 3>;
using bbox_t = std::pair<vec3, vec3>;

template<typename T, int size>
std::string printArray(std::array<T, size> arr){
    std::stringstream out;
    for (int i = 0; i < size; ++i) {
        out << arr[i] << " ";
    }
    out << std::endl;
    return out.str();
}

std::string printVec(glm::vec3 arr){
    std::stringstream out;
    out << arr[0] << " " << arr[1] << " " << arr[2] << std::endl;
    return out.str();
}

template <typename T>
inline std::array<T, 3> computeNormal(std::array<T, 3> x, std::array<T, 3> y) {
    std::array<T, 3> normal;
    normal[0] = x[1] * y[2] - x[2] * y[1];
    normal[1] = x[2] * y[0] - x[0] * y[2];
    normal[2] = x[0] * y[1] - x[1] * y[0];
    return normal;
}

template <typename T, size_t n, size_t m>
inline std::array<T, n+m> concatArray(std::array<T, n> x, std::array<T, m> y){
    std::array<T, n+m> result;
    for (int i = 0; i < n; ++i) {
        result[i] = x[i];
    }
    for (int i = 0; i < m; ++i) {
        result[n+i] = y[i];
    }
    return result;
}

template<typename T, size_t size>
inline std::array<T, size> operator-(std::array<T, size> x, std::array<T, size> y){
    std::array<T, size> result;
    for (int i = 0; i < size; ++i) {
        result[i] = x[i] - y[i];
    }
    return result;
}

template<typename T, size_t size>
inline std::array<T, size> operator+(std::array<T, size> x, std::array<T, size> y){
    std::array<T, size> result;
    for (int i = 0; i < size; ++i) {
        result[i] = x[i] + y[i];
    }
    return result;
}




#endif //LEARNOGL_COMMON_H
