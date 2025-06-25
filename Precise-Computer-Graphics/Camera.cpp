#include "Camera.h"
#include <algorithm>

// Kierunki ruchu kamery
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Domyœlne wartoœci
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM),
    panOffset(0.0f), sceneRotation(glm::mat4(1.0f)) {
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const{
    return glm::lookAt(position + panOffset, position + panOffset + front, up);
}

glm::mat4 Camera::getSceneRotationMatrix() const {
    return sceneRotation;
}

void Camera::processKeyboard(int direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (constrainPitch) {
        pitch = std::max(-89.0f, std::min(pitch, 89.0f));
    }
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    zoom -= yoffset;
    zoom = std::max(1.0f, std::min(zoom, 120.0f));
}

void Camera::rotateScene(float xoffset, float yoffset) {
    float sensitivity = 0.5f;

    // Obrót wokó³ osi Y (yaw)
    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), glm::radians(xoffset * sensitivity), glm::vec3(0.0f, 1.0f, 0.0f));

    // Obrót wokó³ osi X (pitch)
    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), glm::radians(yoffset * sensitivity), glm::vec3(1.0f, 0.0f, 0.0f));

    sceneRotation = yawRotation * pitchRotation * sceneRotation;
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::processMousePan(float xoffset, float yoffset) {
    float sensitivity = 0.005f;
    panOffset -= glm::vec3(xoffset * sensitivity, -yoffset * sensitivity, 0.0f);
}