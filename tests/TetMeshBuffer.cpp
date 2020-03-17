//
// Created by Stefan on 17.03.2020.
//

#include <catch2/catch.hpp>
#include <meshview/TetMeshBuffer.h>
#include <meshview/Window.h>

TEST_CASE("Correctly using Buffers", "[TetMeshBuffers]"){
    // Set up sample data
    std::vector<std::array<float, 3>> vertices;
    vertices.push_back({-0.5, -0.5, -0.5});
    vertices.push_back({ 0.5, -0.5, -0.5});
    vertices.push_back({ 0.0, -0.5,  0.5});
    vertices.push_back({ 0.0,  0.5,  0.0});
    std::vector<std::array<int, 4>> connectivity;
    connectivity.push_back({0, 1, 2, 3});

    // Make window and buffer for testing
    Window window;
    TetMeshBuffer buffer;
    buffer.bufferVertices(vertices, connectivity);
}