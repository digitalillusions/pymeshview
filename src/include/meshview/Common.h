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

#endif //LEARNOGL_COMMON_H
