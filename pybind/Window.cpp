//
// Created by Stefan on 19.03.2020.
//

#include <pybind11/pybind11.h>
#include <meshview/Window.h>

namespace py = pybind11;

void WindowModule(py::module m){
    py::class_<meshview::Window>(m, "Window")
            .def(py::init<>())
            .def(py::init<int, int, bool>(), py::arg("width"), py::arg("height"), py::arg("isVisible") = true)
            // .def("setData", &Window::setData)
            .def("run", &meshview::Window::run, py::arg("nFrames")=0);
}


