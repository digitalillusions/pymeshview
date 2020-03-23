pybind11_add_module(_pymeshview main.cpp Window.cpp TetMeshBuffer.cpp)
target_link_libraries(_pymeshview PRIVATE meshview)