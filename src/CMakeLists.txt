add_library(meshview Camera.cpp ShaderProgram.cpp Window.cpp TetMeshBuffer.cpp)
target_include_directories(meshview PUBLIC include)
target_link_libraries(meshview PUBLIC glfw glm glad stb )

add_library(meshloader MeshLoader.cpp)
target_include_directories(meshloader PUBLIC include)
target_link_libraries(meshloader PRIVATE glm pybind11::embed)

add_custom_command(TARGET meshview POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_BINARY_DIR}/shaders)
