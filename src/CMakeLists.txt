add_library(meshview Camera.cpp MeshLoader.cpp ShaderProgram.cpp)
target_include_directories(meshview PUBLIC include)
target_link_libraries(meshview PUBLIC glfw glm glad stb pybind11::embed)