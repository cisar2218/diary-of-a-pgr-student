#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 direction;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    Camera(const glm::vec3& startPosition, const glm::vec3& startOrientation,
        float startFov, float startAspectRatio, float startNearPlane, float startFarPlane);

    void rotate(float angle, const glm::vec3& axis);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getUp() const;
    glm::vec3 getRight() const;
};
