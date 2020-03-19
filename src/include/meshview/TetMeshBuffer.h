//
// Created by Stefan on 17.03.2020.
//

#ifndef PYMESHVIEWER_TETMESHBUFFER_H
#define PYMESHVIEWER_TETMESHBUFFER_H

#include <meshview/Common.h>
#include <vector>
#include <utility>


namespace meshview {
    class TetMeshBuffer {
    public:
        TetMeshBuffer();
        ~TetMeshBuffer();

        void bufferCells(const std::vector<vec3> &vertices, const std::vector<std::array<int, 4>> &connectivity);
        void glDraw();

        glm::mat4 getModel();
        glm::mat4 getModelInv();
        bbox_t getBbox();

    private:
        void bufferDefaultVertices();
        unsigned int m_vbo, m_vao, m_ebo;
        unsigned int m_nvertices;

        std::vector<vec6> m_vertices_normals;
        bbox_t m_bbox;

        glm::mat4 m_model;
        glm::mat4 m_model_inv;
    };
}


#endif //PYMESHVIEWER_TETMESHBUFFER_H
