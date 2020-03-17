//
// Created by Stefan on 17.03.2020.
//

#ifndef PYMESHVIEWER_TETMESHBUFFER_H
#define PYMESHVIEWER_TETMESHBUFFER_H

#include <meshview/Common.h>
#include <vector>
#include <utility>


class TetMeshBuffer {
public:
    TetMeshBuffer();
    ~TetMeshBuffer();

    void bufferVertices(const std::vector<std::array<float, 3>> &vertices, const std::vector<std::array<int, 4>> &connectivity);
    void glDraw();

private:
    unsigned int m_vbo, m_vao, m_ebo;
    unsigned int n_vertices;

    std::vector<std::array<float, 6>> m_vertices;
};


#endif //PYMESHVIEWER_TETMESHBUFFER_H
