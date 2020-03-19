pybind11_add_module(pymeshview main.cpp Window.cpp TetMeshBuffer.cpp)
target_link_libraries(pymeshview PRIVATE meshview)