//
// Created by Stefan on 19.03.2020.
//

#include <pybind11/pybind11.h>

namespace py = pybind11;

void Window(py::module);
void TetMeshBuffer(py::module);

PYBIND11_MODULE(pymeshview, m){
    m.attr("Hello") = 43;
    Window(m);
    TetMeshBuffer(m);
}
