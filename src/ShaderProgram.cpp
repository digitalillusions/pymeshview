//
// Created by Stefan on 16.03.2020.
//

#include <meshview/ShaderProgram.h>

meshview::ShaderProgram::ShaderProgram() {
    const char * vertex_shader_source = "#version 330 core\n"
                                        "\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "layout (location = 1) in vec3 aNormal;\n"
                                        "\n"
                                        "out vec3 Normal;\n"
                                        "out vec3 FragPos;\n"
                                        "\n"
                                        "uniform mat4 model;\n"
                                        "uniform mat4 modelInv;\n"
                                        "uniform mat4 view;\n"
                                        "uniform mat4 projection;\n"
                                        "\n"
                                        "void main(){\n"
                                        "    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                        "    FragPos = (model * vec4(aPos, 1.0)).xyz;\n"
                                        "    Normal = mat3(transpose(modelInv)) * aNormal;\n"
                                        "}";

    const char * fragment_shader_source = "#version 330 core\n"
                                          "\n"
                                          "in vec3 Normal;\n"
                                          "in vec3 FragPos;\n"
                                          "\n"
                                          "out vec4 FragColor;\n"
                                          "\n"
                                          "uniform vec3 viewPos;\n"
                                          "uniform vec3 lightPos;\n"
                                          "// uniform vec3 lightColor;\n"
                                          "// uniform vec3 objectColor;\n"
                                          "\n"
                                          "\n"
                                          "void main(){\n"
                                          "    vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"
                                          "    vec3 objectColor = vec3(0.8, 0.8, 0.8);\n"
                                          "\n"
                                          "    float ambientStrength = 0.4;\n"
                                          "    vec3 ambient = ambientStrength * lightColor;\n"
                                          "\n"
                                          "    float diffuseStrength = 0.8;\n"
                                          "    vec3 norm = normalize(Normal);\n"
                                          "    vec3 lightDir = normalize(viewPos - FragPos);\n"
                                          "    // float diffuseIntensity = max(dot(norm, lightDir), 0.0);\n"
                                          "    float diffuseIntensity = abs(dot(norm, lightDir));\n"
                                          "    vec3 diffuse = diffuseIntensity * diffuseStrength * lightColor;\n"
                                          "\n"
                                          "    float specularStrength = 0.2;\n"
                                          "    vec3 viewDir = normalize(viewPos - FragPos);\n"
                                          "    vec3 reflectDir = reflect(-lightDir, norm);\n"
                                          "    float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
                                          "    vec3 specular = specularIntensity * specularStrength * lightColor;\n"
                                          "\n"
                                          "    vec3 result = objectColor * (ambient + diffuse + specular);\n"
                                          "    FragColor = vec4(result, 1.0);\n"
                                          "}";

    compileShaderProgram(vertex_shader_source, fragment_shader_source);
}


meshview::ShaderProgram::ShaderProgram(const char * vertexShader, const char* fragmentShader, const char* geometryShader){
    std::string vertex_source, fragment_source, geometry_source;
    std::ifstream vertex_fstream, fragment_fstream, geometry_fstream;

    vertex_fstream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    fragment_fstream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    geometry_fstream.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    try {
        // Open file
        vertex_fstream.open(vertexShader);
        fragment_fstream.open(fragmentShader);

        // Read file
        std::stringstream vertex_sstream, fragment_sstream;
        vertex_sstream << vertex_fstream.rdbuf();
        fragment_sstream << fragment_fstream.rdbuf();

        // Close file
        vertex_fstream.close();
        fragment_fstream.close();

        // Get string of the stringstream
        vertex_source = vertex_sstream.str();
        fragment_source = fragment_sstream.str();

        // Handle geometry m_shader if it exists
        if (geometryShader != nullptr){
            geometry_fstream.open(geometryShader);
            std::stringstream geometry_sstream;
            geometry_sstream << geometry_fstream.rdbuf();
            geometry_fstream.close();
            geometry_source = geometry_sstream.str();
        }
    } catch (std::ifstream::failure ex){
        std::cout << "Error while reading m_shader! Error code: " << ex.what() << std::endl;
    }

    const char * vertex_source_cstr = vertex_source.c_str();
    const char * fragment_source_cstr = fragment_source.c_str();
    const char * geometry_source_cstr = geometryShader != nullptr ? geometry_source.c_str() : nullptr;


    compileShaderProgram(vertex_source_cstr, fragment_source_cstr, geometry_source_cstr);

}

void meshview::ShaderProgram::compileShaderProgram(const char *vertex_source_cstr, const char *fragment_source_cstr,
                                                   const char *geometry_source_cstr) {
    int success;
    char infoLog[512];

    // Compile vertex m_shader and check for errors
    unsigned int vertex_shader_id, fragment_shader_id, geometry_shader_id;
    vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_source_cstr, NULL);
    glCompileShader(vertex_shader_id);

    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertex_shader_id, 512, NULL, infoLog);
        std::cout << "Vertex m_shader compilation failed: " << infoLog << std::endl;
    }

    // Compile fragment m_shader and check for errors
    fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_source_cstr, NULL);
    glCompileShader(fragment_shader_id);

    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragment_shader_id, 512, NULL, infoLog);
        std::cout << "Fragment m_shader compilation failed: " << infoLog << std::endl;
    }

    if (geometry_source_cstr != nullptr){
        geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry_shader_id, 1, &geometry_source_cstr, NULL);
        glCompileShader(geometry_shader_id);
        glGetShaderiv(geometry_shader_id, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(geometry_shader_id, 512, NULL, infoLog);
            std::cout << "Geometry m_shader compilation failed: " << infoLog << std::endl;
        }
    }

    // Make the program, attach shaders, link the program and check for errors
    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertex_shader_id);
    if (geometry_source_cstr != nullptr){
        glAttachShader(this->programID, geometry_shader_id);
    }
    glAttachShader(this->programID, fragment_shader_id);
    glLinkProgram(this->programID);

    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(this->programID, 512, NULL, infoLog);
        std::cout << "Linking program failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    if (geometry_source_cstr != nullptr){
        glDeleteShader(geometry_shader_id);
    }
}

void meshview::ShaderProgram::use() {
    glUseProgram(programID);
}

// Get the program ID
unsigned int meshview::ShaderProgram::getProgramID() const{
    return programID;
};

void meshview::ShaderProgram::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void meshview::ShaderProgram::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void meshview::ShaderProgram::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void meshview::ShaderProgram::setVec3fv(const std::string &name, const float *value) const{
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, value);
}

void meshview::ShaderProgram::setVec4fv(const std::string &name, const float *value) const{
    glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, value);
}

void meshview::ShaderProgram::setMat3fv(const std::string &name, const float *value) const{
    glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
}

void meshview::ShaderProgram::setMat4fv(const std::string &name, const float *value) const{
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
}
