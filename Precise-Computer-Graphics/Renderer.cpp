#include "Renderer.h"
#include <iostream>
#include <cmath>

const char* vertexShaderSource = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
)glsl";

Renderer::Renderer() : basicShader(nullptr) {}

Renderer::~Renderer() {
    cleanup();
}

void Renderer::initialize() {
    basicShader = new Shader(vertexShaderSource, fragmentShaderSource);

    setupTriangle();
    setupAxis();
    setupGrid();
    setupCube();
}

void Renderer::setupTriangle() {
    float vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)), 0.0f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f * float(sqrt(3)), 0.0f,  1.0f, 1.0f, 0.0f,
         0.0f,  0.5f * float(sqrt(3)) * 2.0f / 3.0f, 0.0f,  1.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::setupAxis() {
    float axisVertices[] = {
        // X axis (red)
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

        // Y axis (green)
        0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,

        // Z axis (blue)
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::setupGrid() {
    gridVertices = generateGrid(20);

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::setupCube() {
    float cubeVertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

        // Back face
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,

        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

        // Right face
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

         // Bottom face
         -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,

         // Top face
         -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

std::vector<float> Renderer::generateGrid(int halfSize, float spacing) {
    std::vector<float> vertices;

    for (int i = -halfSize; i <= halfSize; ++i) {
        float coord = i * spacing;

        // Linie równoleg³e do X
        vertices.insert(vertices.end(), {
            -halfSize * spacing, 0.0f, coord,  0.5f, 0.5f, 0.5f,
             halfSize * spacing, 0.0f, coord,  0.5f, 0.5f, 0.5f
            });

        // Linie równoleg³e do Z
        vertices.insert(vertices.end(), {
            coord, 0.0f, -halfSize * spacing,  0.5f, 0.5f, 0.5f,
            coord, 0.0f,  halfSize * spacing,  0.5f, 0.5f, 0.5f
            });
    }

    return vertices;
}

void Renderer::render(const Camera& camera, float aspectRatio) {
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), aspectRatio, 0.1f, 100.0f);
    glm::mat4 sceneRotation = camera.getSceneRotationMatrix();

    basicShader->use();
    basicShader->setMat4("view", view);
    basicShader->setMat4("projection", projection);

    // Renderuj siatkê z rotacj¹ sceny
    glm::mat4 model = sceneRotation;
    basicShader->setMat4("model", model);
    glBindVertexArray(gridVAO);
    glDrawArrays(GL_LINES, 0, gridVertices.size() / 6);

    // Renderuj osie z rotacj¹ sceny
    basicShader->setMat4("model", model);
    glBindVertexArray(axisVAO);
    glDrawArrays(GL_LINES, 0, 6);

    // Renderuj trójk¹t z rotacj¹ sceny
    basicShader->setMat4("model", model);
    glBindVertexArray(triangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::renderGizmo(const Camera& camera, float x, float y, float size) {
    // Zapisz aktualny viewport
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Ustaw viewport dla gizmo
    glViewport(x, y, size, size);

    // Ustaw macierze dla gizmo
    glm::mat4 gizmoView = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 gizmoProjection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    glm::mat4 gizmoModel = camera.getSceneRotationMatrix() * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    // Wyczyœæ bufor g³êbokoœci dla gizmo
    glClear(GL_DEPTH_BUFFER_BIT);

    basicShader->use();
    basicShader->setMat4("view", gizmoView);
    basicShader->setMat4("projection", gizmoProjection);
    basicShader->setMat4("model", gizmoModel);

    // Renderuj kostke gizmo
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Przywróæ oryginalny viewport
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void Renderer::cleanup() {
    if (basicShader) {
        delete basicShader;
        basicShader = nullptr;
    }

    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteBuffers(1, &triangleVBO);
    glDeleteVertexArrays(1, &axisVAO);
    glDeleteBuffers(1, &axisVBO);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
}