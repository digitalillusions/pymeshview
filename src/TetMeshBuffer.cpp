//
// Created by Stefan on 17.03.2020.
//

#include "meshview/TetMeshBuffer.h"

TetMeshBuffer::TetMeshBuffer() {
    n_vertices = 0;
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenVertexArrays(1, &m_vao);
}

TetMeshBuffer::~TetMeshBuffer() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void TetMeshBuffer::bufferVertices(const std::vector<std::array<float, 3>> &vertices,
                                   const std::vector<std::array<int, 4>> &connectivity) {

}

void TetMeshBuffer::glDraw() {

}
