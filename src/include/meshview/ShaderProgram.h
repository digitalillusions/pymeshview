//
// Created by sjeske on 3/9/20.
//
#include <glad/glad.h>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#ifndef LEARNOGL_SHADERPROGRAM_H
#define LEARNOGL_SHADERPROGRAM_H

class ShaderProgram{
public:
    // Compile vertex and fragment shader and make a shader program
    ShaderProgram(const char * vertexShader, const char * fragmentShader, const char * geometryShader = nullptr);

    // Activate the shader for usage in opengl
    void use();

    // Get the program ID
    unsigned int getProgramID() const;;

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setVec3fv(const std::string &name, const float *value) const;

    void setVec4fv(const std::string &name, const float *value) const;

    void setMat3fv(const std::string &name, const float *value) const;

    void setMat4fv(const std::string &name, const float *value) const;

private:
    unsigned int programID;
};



#endif //LEARNOGL_SHADERPROGRAM_H
