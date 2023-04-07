#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
    this->upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(const glm::vec3& startPosition, const glm::vec3& startDirection, const glm::vec3& worldUp) {
    this->position = startPosition;
    this->direction = startDirection;
    this->upVector = worldUp;
}

void Camera::rotate(float angle, const glm::vec3& axis)
{
    // TODO need to be implemented
}

void Camera::move(glm::vec3 movement) {
    this->position += movement;
}

void Camera::moveRight(float movement) {
    glm::vec3 left = glm::cross(this->direction, this->upVector);
    this->move(left * -movement);
}

void Camera::moveLeft(float movement)
{
    glm::vec3 left = glm::cross(this->direction, this->upVector);
    this->move(left * movement);
}

void Camera::moveForward(float movement)
{
    this->move(direction * -movement);
}

void Camera::moveBackward(float movement)
{
    this->move(direction * movement);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(
        this->position,
        this->position - this->direction,
        this->upVector
    );
}

glm::mat4 Camera::getProjectionMatrix() const {
    return this->projectionMatrix;
}