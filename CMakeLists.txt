cmake_minimum_required(VERSION 3.3)

project(PyMeshViewer CXX C)

set(CMAKE_CXX_STANDARD 14)

option(MESHVIEW_TEST "Include test targets" ON)

if (MINGW AND CMAKE_BUILD_TYPE MATCHES DEBUG)
    add_compile_options(-Wa,-mbig-obj)
endif ()
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add external dependencies
add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)
add_subdirectory(extern/glm)
add_subdirectory(extern/pybind11)
add_subdirectory(extern/stb)
add_subdirectory(extern/Catch2)

# Add main source code
add_subdirectory(src)

# Add python bindings
add_subdirectory(pybind)

# Add example code executables
add_subdirectory(examples)

# Add tests of the option is set
if (MESHVIEW_TEST)
    add_subdirectory(tests)
endif ()