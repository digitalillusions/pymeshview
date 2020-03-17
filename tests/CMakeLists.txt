add_executable(meshview_test main.cpp Window.cpp TetMeshBuffer.cpp)
target_link_libraries(meshview_test meshview Catch2)