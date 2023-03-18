#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovementDir {
    UP, DOWN, LEFT, RIGHT
};

class Camera {
    private:
        glm::vec3 m_position;
        glm::vec3 m_forwards;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;

        float m_yaw;
        float m_pitch;

        float m_movementSpeed;
        float m_mouseSensitivity;
        float m_fov;
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                float yaw = -90.0f, float pitch = 0.0f);
        ~Camera() {}

        glm::mat4 GetCameraView();
        void ProcessKeyboardInput(CameraMovementDir dir, float deltaTime);
        void ProcessMouseInput(float xOffset, float yOffset);
        void ProcessScrollInput(float yScroll);
        void UpdateVectors();

        float GetFOV();
        glm::vec3 GetPosition();
        glm::vec3 GetForwards();
};