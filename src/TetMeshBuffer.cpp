//
// Created by Stefan on 17.03.2020.
//

#include <meshview/TetMeshBuffer.h>

meshview::TetMeshBuffer::TetMeshBuffer() {
    // Init default parameters
    m_nvertices = 0;
    m_bbox = std::make_pair(vec3{0.0, 0.0, 0.0}, vec3{0.0, 0.0, 0.0});
    m_vertices_normals.clear();
    m_model = glm::mat4(1.0);
    m_model_inv = glm::mat4(1.0);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenVertexArrays(1, &m_vao);

    bufferDefaultVertices();
}

meshview::TetMeshBuffer::~TetMeshBuffer() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void meshview::TetMeshBuffer::bufferCells(const std::vector<vec3> &vertices,
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

    // Compute the bounding box
    m_bbox = computeBoundingBox(vertices);

    // Bind the buffers
    bindBuffers();
}

void meshview::TetMeshBuffer::bufferVerticesNormals(const std::vector<vec6> &vertices_normals, bbox_t bound_box) {
    // Assign vertices and normals
    m_vertices_normals = vertices_normals;

    // Assign bounding box
    m_bbox = bound_box;

    // Bind the buffers
    bindBuffers();
}

void meshview::TetMeshBuffer::bindBuffers() {
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

    // Disable vertex array
    glBindVertexArray(0);
}

void meshview::TetMeshBuffer::glDraw() {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_nvertices);
    glBindVertexArray(0);
}

glm::mat4 meshview::TetMeshBuffer::getModel() {
    return m_model;
}

glm::mat4 meshview::TetMeshBuffer::getModelInv() {
    return m_model_inv;
}

bbox_t meshview::TetMeshBuffer::getBbox() {
    return m_bbox;
}

void meshview::TetMeshBuffer::bufferDefaultVertices() {
    // Set up default data
    std::vector<std::array<float, 3>> vertices;
    vertices.push_back({-0.5, -0.5, -0.5});
    vertices.push_back({ 0.5, -0.5, -0.5});
    vertices.push_back({ 0.0, -0.5,  0.5});
    vertices.push_back({ 0.0,  0.5,  0.0});
    std::vector<std::array<int, 4>> connectivity;
    connectivity.push_back({0, 1, 2, 3});

    // Add it to the buffers
    bufferCells(vertices, connectivity);
}
