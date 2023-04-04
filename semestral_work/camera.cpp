#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& startPosition, const glm::vec3& startOrientation,
    float startFov, float startAspectRatio, float startNearPlane, float startFarPlane) {
    this->position = startPosition;
    this->direction = startOrientation;
    this->fov = startFov;
    this->aspectRatio = startAspectRatio;
    this->nearPlane = startNearPlane;
    this->farPlane = startFarPlane;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + direction, getUp());
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::rotate(float angle, const glm::vec3& axis) {
    glm::quat rotation = glm::angleAxis(glm::radians(angle), axis);
    direction = glm::normalize(rotation * direction);
}

glm::vec3 Camera::getUp() const {
    return glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::getRight() const {
    return glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
}

