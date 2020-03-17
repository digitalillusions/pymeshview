add_executable(test main.cpp Window.cpp TetMeshBuffer.cpp)
target_link_libraries(test meshview Catch2)