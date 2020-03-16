cmake_minimum_required(VERSION 3.3)

project(PyMeshViewer CXX C)

# Add external dependencies
add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)
add_subdirectory(extern/glm)
add_subdirectory(extern/pybind11)

# Add main source code
add_subdirectory(src)

# Add example code executables
add_subdirectory(examples)