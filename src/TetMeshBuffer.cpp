//
// Created by Stefan on 17.03.2020.
//

#include "meshview/TetMeshBuffer.h"

TetMeshBuffer::TetMeshBuffer() {
    m_nvertices = 0;
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenVertexArrays(1, &m_vao);
}

TetMeshBuffer::~TetMeshBuffer() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void TetMeshBuffer::bufferVertices(const std::vector<vec3> &vertices,
                                   const std::vector<std::array<int, 4>> &connectivity) {
    // Compute the triangle combinations on the element
    std::vector<std::array<unsigned int, 4>> tet_combs;
    tet_combs.push_back({0, 1, 2});
    tet_combs.push_back({0, 3, 1});
    tet_combs.push_back({0, 2, 3});
    tet_combs.push_back({1, 3, 2});

    // Compute normal for every element and save the triangles with corresponding normals
    for(const auto & elem : connectivity){
        for(const auto & ch : tet_combs){
            auto normal = computeNormal(vertices[elem[ch[1]]] - vertices[elem[ch[0]]], vertices[elem[ch[2]]] - vertices[elem[ch[0]]]);
            m_vertices_normals.push_back(concatArray(vertices[elem[ch[0]]], normal));
            m_vertices_normals.push_back(concatArray(vertices[elem[ch[1]]], normal));
            m_vertices_normals.push_back(concatArray(vertices[elem[ch[2]]], normal));
        }
    }

    // Store the effective number of vertices
    m_nvertices = m_vertices_normals.size();

    // Bind the vertex array and copy the data
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * m_nvertices, &m_vertices_normals[0][0], GL_STATIC_DRAW);

    // Position data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

}

void TetMeshBuffer::glDraw() {

}
