//
// Created by sjeske on 3/13/20.
//

#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <meshview/MeshLoader.h>
#include <iostream>

MeshLoader::MeshLoader(const std::string filename, CellType cell_type) : m_cell_type(cell_type){
    py::scoped_interpreter m_guard{};
    std::cout << "Attempting to load module." << std::endl;
    auto meshio = py::module::import("meshio");
    std::cout << "Loaded Meshio Module." << std::endl;
    auto data = meshio.attr("read")(filename);
    m_vertices.clear();
    auto x = data.attr("points").cast<std::vector<vec3>>();
    m_vertices = data.attr("points").cast<std::vector<vec3>>();
    computeCombinations();
    if (cell_type == CellType::TETRAHEDRAL){
        auto cells = data.attr("cells").attr("__getitem__")(0).attr("data").cast<std::vector<std::array<unsigned int, 4>>>();
        m_cells.clear();
        for(const auto & elem : cells){
            // Add all the cell permutations
            m_cells.push_back({elem[0], elem[1], elem[2]});
            m_cells.push_back({elem[0], elem[3], elem[1]});
            m_cells.push_back({elem[0], elem[2], elem[3]});
            m_cells.push_back({elem[1], elem[3], elem[2]});

            // Add all the cell permutations as vertices
            for(const auto & comb : m_combs){
                auto normal = computeNormal(x[elem[comb[1]]] - x[elem[comb[0]]], x[elem[comb[2]]]- x[elem[comb[0]]]);
                m_verts_and_normals.push_back(concatArray(x[elem[comb[0]]], normal));
                m_verts_and_normals.push_back(concatArray(x[elem[comb[1]]], normal));
                m_verts_and_normals.push_back(concatArray(x[elem[comb[2]]], normal));
            }
        }
    }
    py::print("Mesh Loaded Successfully");
    computeBoundingBox();

    float* vertex_ptr = &m_vertices[0][0];
    // Verify data pointer correctness
    int last_vec = m_vertices.size()-1;
    std::cout << "First Vector: " << vertex_ptr[0] << " " << vertex_ptr[1] << " " << vertex_ptr[2] << std::endl;
    std::cout << "First Vector check: " << m_vertices[0][0] << " " << m_vertices[0][1] << " " << m_vertices[0][2] << std::endl;
    std::cout << "Second Vector: " << vertex_ptr[3] << " " << vertex_ptr[4] << " " << vertex_ptr[5] << std::endl;
    std::cout << "Second Vector check: " << m_vertices[1][0] << " " << m_vertices[1][1] << " " << m_vertices[1][2] << std::endl;
    std::cout << "Last Vector: " << vertex_ptr[3*last_vec] << " " << vertex_ptr[3*last_vec + 1] << " " << vertex_ptr[3*last_vec + 2] << std::endl;
    std::cout << "Last Vector check: " << m_vertices[last_vec][0] << " " << m_vertices[last_vec][1] << " " << m_vertices[last_vec][2] << std::endl;
}

void MeshLoader::testPointers() {
    std::cout << "First cell consisting of points:" << std::endl;
    for (const auto & index : m_cells[0]){
        std::cout << m_vertices[index][0] << " " << m_vertices[index][1] << " " << m_vertices[index][2] << std::endl;
    }
    std::cout << "First cell consisting of triangles:" << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << m_verts_and_normals[i][0] << " " <<  m_verts_and_normals[i][1] << " " << m_verts_and_normals[i][2] << std::endl;
    }
}

float *MeshLoader::getVertices() {
    return &m_vertices[0][0];
}

unsigned int MeshLoader::getNVertices() {
    return m_vertices.size();
}

unsigned int *MeshLoader::getCells() {
    return &m_cells[0][0];
}

unsigned int MeshLoader::getNCells() {
    return m_cells.size();
}

float *MeshLoader::getVertsAndNormals() {
    return &m_verts_and_normals[0][0];
}

unsigned int MeshLoader::getNVertsAndNormals() {
    return m_verts_and_normals.size();
}

void MeshLoader::computeCombinations() {
    m_combs.push_back({0, 1, 2});
    m_combs.push_back({0, 3, 1});
    m_combs.push_back({0, 2, 3});
    m_combs.push_back({1, 3, 2});
}

std::pair<glm::vec3, glm::vec3> MeshLoader::getBoundingBox() {
    return std::pair<glm::vec3, glm::vec3>(m_bbox_min, m_bbox_max);
}

void MeshLoader::computeBoundingBox() {
    m_bbox_min = glm::vec3(0,0,0);
    m_bbox_max = glm::vec3(0,0,0);
    for(const auto & elem: m_vertices){
        for (int i = 0; i < 3; ++i) {
            if(elem[i] > m_bbox_max[i]){
                m_bbox_max[i] = elem[i];
            } else if (elem[i] < m_bbox_min[i]){
                m_bbox_min[i] = elem[i];
            }
        }
    }
}

