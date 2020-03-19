//
// Created by sjeske on 3/13/20.
//

#ifndef LEARNOGL_MESHLOADER_H
#define LEARNOGL_MESHLOADER_H

#include <meshview/Common.h>
#include <vector>
#include <array>
#include <pybind11/pybind11.h>
#include <glm/glm.hpp>
namespace py = pybind11;

namespace meshview {
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
        std::vector<vec3> m_vertices;
        std::vector<vec6> m_verts_and_normals;
        std::vector<vec3ui> m_cells;
        CellType m_cell_type;

        std::vector<vec3ui> m_combs;
        glm::vec3 m_bbox_min, m_bbox_max;

    };
}


#endif //LEARNOGL_MESHLOADER_H
