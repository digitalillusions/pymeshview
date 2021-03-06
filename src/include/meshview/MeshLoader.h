//
// Created by sjeske on 3/13/20.
//

#ifndef LEARNOGL_MESHLOADER_H
#define LEARNOGL_MESHLOADER_H

#include <meshview/Common.h>
#include <vector>
#include <array>
#include <glm/glm.hpp>

namespace meshview {
    enum CellType{
        TETRAHEDRAL = 0
    };

    class MeshLoader {
    public:
        MeshLoader();
        MeshLoader(const std::string filename, CellType cell_type=CellType::TETRAHEDRAL);
        void testPointers();
        float* getVertices();
        unsigned int getNVertices();
        unsigned int * getCells();
        unsigned int getNCells();
        float* getVertsAndNormals();
        unsigned int getNVertsAndNormals();
        std::pair<glm::vec3, glm::vec3> getBoundingBox();
        bbox_t getBoundingBoxArray();

    private:
        std::string openFile();
        void loadMesh(std::string filename);
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
