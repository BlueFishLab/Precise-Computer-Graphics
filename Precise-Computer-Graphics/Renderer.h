#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Camera.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void initialize();
    void render(const Camera& camera, float aspectRatio);
    void cleanup();

    void renderGizmo(const Camera& camera, float x, float y, float size);

private:
    // Shaders
    Shader* basicShader;

    // VAOs and VBOs
    unsigned int triangleVAO, triangleVBO;
    unsigned int axisVAO, axisVBO;
    unsigned int gridVAO, gridVBO;
    unsigned int cubeVAO, cubeVBO;

    // Vertex data
    std::vector<float> gridVertices;

    void setupTriangle();
    void setupAxis();
    void setupGrid();
    void setupCube();
    std::vector<float> generateGrid(int halfSize, float spacing = 1.0f);
};