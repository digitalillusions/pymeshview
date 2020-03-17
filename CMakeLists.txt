cmake_minimum_required(VERSION 3.3)

project(PyMeshViewer CXX C)

option(MESHVIEW_TEST "Include test targets" ON)

if (MINGW)
    add_compile_options(-Wa,-mbig-obj)
endif ()

# Add external dependencies
add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)
add_subdirectory(extern/glm)
add_subdirectory(extern/pybind11)
add_subdirectory(extern/stb)
add_subdirectory(extern/Catch2)

# Add main source code
add_subdirectory(src)

# Add example code executables
add_subdirectory(examples)

# Add tests of the option is set
if (MESHVIEW_TEST)
    add_subdirectory(tests)
endif ()