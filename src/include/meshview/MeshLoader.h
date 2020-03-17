//
// Created by sjeske on 3/13/20.
//

#ifndef LEARNOGL_MESHLOADER_H
#define LEARNOGL_MESHLOADER_H

#include "Common.h"
#include <vector>
#include <array>
#include <pybind11/pybind11.h>
#include <glm/glm.hpp>
namespace py = pybind11;

enum CellType{
    TETRAHEDRAL = 0
};

class MeshLoader {
public:
    MeshLoader(const std::string filename, CellType cell_type);
    void testPointers();
    float* getVertices();
    unsigned int getNVertices();
    unsigned int * getCells();
    unsigned int getNCells();
    float* getVertsAndNormals();
    unsigned int getNVertsAndNormals();
    std::pair<glm::vec3, glm::vec3> getBoundingBox();

private:
    void computeCombinations();
    void computeBoundingBox();
    inline std::array<float, 3> computeNormal(std::array<float, 3> x, std::array<float, 3> y);
    inline std::array<float, 3> subtractArray(std::array<float, 3> x, std::array<float, 3> y);
    inline std::array<float, 6> concatArray(std::array<float, 3> x, std::array<float, 3> y);
    std::vector<std::array<float, 3>> m_vertices;
    std::vector<std::array<float, 6>> m_verts_and_normals;
    std::vector<std::array<unsigned int, 3>> m_cells;
    CellType m_cell_type;

    std::vector<std::array<unsigned int, 3>> m_combs;
    glm::vec3 m_bbox_min, m_bbox_max;

};


#endif //LEARNOGL_MESHLOADER_H
