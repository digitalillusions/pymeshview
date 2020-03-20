//
// Created by Stefan on 19.03.2020.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <memory>
#include <meshview/TetMeshBuffer.h>

namespace py = pybind11;

void TetMeshBuffer(py::module m){
    py::class_<meshview::TetMeshBuffer, std::shared_ptr<meshview::TetMeshBuffer>>(m, "TetMeshBuffer")
            .def(py::init<>())
            .def("bufferCells", &meshview::TetMeshBuffer::bufferCells);
}