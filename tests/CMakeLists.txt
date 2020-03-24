add_executable(meshtest main.cpp Window.cpp TetMeshBuffer.cpp)
target_link_libraries(meshtest meshview Catch2)