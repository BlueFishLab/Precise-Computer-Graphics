#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"

// Globalne zmienne
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Renderer renderer;

// Ustawienia okna
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Mouse input
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// Mouse states
bool middleMousePressed = false;
bool rightMousePressed = false;
bool gizmoInteraction = false;

// Gizmo settings
const float GIZMO_SIZE = 100.0f;
const float GIZMO_MARGIN = 20.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
bool isMouseOverGizmo(double mouseX, double mouseY);

int main() {
    // Inicjalizacja GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Utworzenie okna
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL 3D Viewer", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Ustawienie callback'ów
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Inicjalizacja GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Konfiguracja OpenGL
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Inicjalizacja renderera
    renderer.initialize();

    // Inicjalizacja ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // G³ówna pêtla renderowania
    while (!glfwWindowShouldClose(window)) {
        // Timing
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Render
        glClearColor(0.12f, 0.13f, 0.13f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Renderowanie sceny
        renderer.render(camera, (float)SCR_WIDTH / (float)SCR_HEIGHT);

        // Renderowanie gizmo w prawym górnym rogu
        float gizmoX = SCR_WIDTH - GIZMO_SIZE - GIZMO_MARGIN;
        float gizmoY = SCR_HEIGHT - GIZMO_SIZE - GIZMO_MARGIN;
        renderer.renderGizmo(camera, gizmoX, gizmoY, GIZMO_SIZE);

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Panel kontrolny
        ImGui::Begin("Kontrola kamery");
        ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
        ImGui::Separator();

        ImGui::Text("Sterowanie:");
        ImGui::BulletText("WASD - ruch kamery");
        ImGui::BulletText("Œrodkowy przycisk myszy - przesuwanie");
        ImGui::BulletText("Prawy przycisk myszy - obracanie kamery");
        ImGui::BulletText("Kostka w rogu - obracanie sceny");
        ImGui::BulletText("Scroll - zoom");

        ImGui::Separator();
        ImGui::Text("Pozycja kamery: %.2f, %.2f, %.2f",
            camera.position.x, camera.position.y, camera.position.z);
        ImGui::Text("FOV: %.1f", camera.zoom);

        if (ImGui::Button("Reset kamery")) {
            camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers i poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    renderer.cleanup();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Sterowanie kamer¹ WASD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(0, deltaTime); // FORWARD
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(1, deltaTime); // BACKWARD
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(2, deltaTime); // LEFT
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(3, deltaTime); // RIGHT
}

bool isMouseOverGizmo(double mouseX, double mouseY) {
    float gizmoX = SCR_WIDTH - GIZMO_SIZE - GIZMO_MARGIN;
    float gizmoY = SCR_HEIGHT - GIZMO_SIZE - GIZMO_MARGIN;

    // Konwersja wspó³rzêdnych myszy (GLFW u¿ywa lewego górnego rogu, OpenGL lewego dolnego)
    double oglMouseY = SCR_HEIGHT - mouseY;

    return (mouseX >= gizmoX && mouseX <= gizmoX + GIZMO_SIZE &&
        oglMouseY >= gizmoY && oglMouseY <= gizmoY + GIZMO_SIZE);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            middleMousePressed = true;
            lastX = mouseX;
            lastY = mouseY;
        }
        else if (action == GLFW_RELEASE) {
            middleMousePressed = false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMousePressed = true;
            gizmoInteraction = isMouseOverGizmo(mouseX, mouseY);
            lastX = mouseX;
            lastY = mouseY;

            if (!gizmoInteraction) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
        else if (action == GLFW_RELEASE) {
            rightMousePressed = false;
            gizmoInteraction = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Odwrócone, bo wspó³rzêdne y id¹ od do³u do góry

    lastX = xpos;
    lastY = ypos;

    // Œrodkowy przycisk myszy - przesuwanie
    if (middleMousePressed) {
        camera.processMousePan(xoffset, yoffset);
    }

    // Prawy przycisk myszy
    if (rightMousePressed) {
        if (gizmoInteraction) {
            // Obracanie sceny przez gizmo
            camera.rotateScene(xoffset, yoffset);
        }
        else {
            // Obracanie kamery
            camera.processMouseMovement(xoffset, yoffset);
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(static_cast<float>(yoffset));
}