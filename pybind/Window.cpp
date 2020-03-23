//
// Created by Stefan on 19.03.2020.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <meshview/Window.h>

namespace py = pybind11;

void Window(py::module m){
    py::class_<meshview::Window>(m, "Window")
            .def(py::init<>())
            .def(py::init<int, int, bool>(), py::arg("width")=800, py::arg("height")=600, py::arg("isVisible") = true)
            .def("setData", &meshview::Window::setData)
            .def("highlight", &meshview::Window::highlight, py::arg("axis")=vec3{0.0, 0.0, 1.0},
                    py::arg("radius")=4, py::arg("prefixPath")=".")
            .def("run", &meshview::Window::run, py::arg("nFrames")=0);
}


