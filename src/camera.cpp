#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
m_forwards(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(5.0f), m_mouseSensitivity(0.1f), m_fov(40.0f) {
    m_position = position;
    m_worldUp = up;
    m_yaw = yaw;
    m_pitch = pitch;

    UpdateVectors();
}

glm::mat4 Camera::GetCameraView() {
    return glm::lookAt(m_position, m_position + m_forwards, m_up);
}

void Camera::ProcessKeyboardInput(CameraMovementDir dir, float deltaTime) {
    float cameraSpeed = m_movementSpeed * deltaTime;

    if (dir == UP)
        m_position += cameraSpeed * m_forwards;
    if (dir == DOWN)
        m_position -= cameraSpeed * m_forwards;
    if (dir == LEFT)
        m_position -= cameraSpeed * glm::normalize(glm::cross(m_forwards, m_up));
    if (dir == RIGHT)
        m_position += cameraSpeed * glm::normalize(glm::cross(m_forwards, m_up));
}

void Camera::ProcessMouseInput(float xOffset, float yOffset) {
    m_yaw += xOffset * m_mouseSensitivity;
    m_pitch += yOffset * m_mouseSensitivity;

    if (m_pitch > 89.0f) { m_pitch = 89.0f; }
    if (m_pitch < -89.0f) { m_pitch = -89.0f; }

    UpdateVectors();
}

void Camera::ProcessScrollInput(float yScroll) {
    m_fov -= yScroll;

    if (m_fov > 60.0f) { m_fov = 60.0f; }
    else if (m_fov < 5.0f) { m_fov = 5.0f; }
}

void Camera::UpdateVectors() {
    glm::vec3 frontDir;
    frontDir.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    frontDir.y = sin(glm::radians(m_pitch));
    frontDir.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_forwards = glm::normalize(frontDir);
    m_right = glm::normalize(glm::cross(m_forwards, m_worldUp));
    m_up  = glm::normalize(glm::cross(m_right, m_forwards));
}

float Camera::GetFOV() {
    return glm::radians(m_fov);
}

glm::vec3 Camera::GetPosition() {
    return m_position;
}

glm::vec3 Camera::GetForwards() {
    return m_forwards;
}