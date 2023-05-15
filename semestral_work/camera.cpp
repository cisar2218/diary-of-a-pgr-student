#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "parameters.h"
#include <iostream>
using namespace std;

Camera::Camera()
{
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
    this->upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(const glm::vec3& startPosition) {
    this->position = startPosition;
    this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
    this->upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(const glm::vec3& startPosition, const glm::vec3& startDirection) {
    this->position = startPosition;
    this->direction = glm::normalize(startDirection);
    this->upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(const glm::vec3& startPosition, const glm::vec3& startDirection, const glm::vec3& worldUp) {
    this->position = startPosition;
    this->direction = glm::normalize(startDirection);
    this->upVector = glm::normalize(worldUp);
}

void Camera::setCameraFrom(Camera other)
{
    this->projectionMatrix = other.projectionMatrix;
    this->elevationAngle = other.elevationAngle;
    this->position = other.position;
    this->direction = other.direction;
    this->upVector = other.upVector;
}

void Camera::setRefObject(shared_ptr<MovingObject>  newRefObj)
{
    this->refObj = newRefObj;
}

void Camera::setPositionAsReferObj()
{
    if (this->refObj != nullptr) {
        this->position = refObj->getPosition();
        this->position += 3.0f;
        this->direction = -1.0f * refObj->getDirection();
    }
}

void Camera::rotateHorizontal(float angle)
{
    glm::mat2x2 rotMat = glm::mat2(
        glm::vec2(cos(angle), sin(angle)), 
        glm::vec2(-sin(angle), cos(angle))
    );
    glm::vec2 rotated2dVec = rotMat * glm::vec2(this->direction.x, this->direction.z);
    this->direction = glm::normalize(glm::vec3(rotated2dVec.x, 0.0f ,rotated2dVec.y));
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

void Camera::setPosition(glm::vec3 newPosition)
{
    this->position = newPosition;
}

void Camera::ground()
{
    this->position.y = 0.0f;
}

glm::vec3 Camera::getFront() {
    return this->direction;
}

glm::vec3 Camera::getLeft() {
    return glm::cross(this->direction, this->upVector);
}

void Camera::increaseElevation(float angle)
{
    this->elevationAngle -= angle;
    if (this->elevationAngle > CAMERA_FREE_ELEVATION_MAX) {
        this->elevationAngle = CAMERA_FREE_ELEVATION_MAX;
    }
    else if (this->elevationAngle < -CAMERA_FREE_ELEVATION_MAX) {
        this->elevationAngle = -CAMERA_FREE_ELEVATION_MAX;
    }
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(
        this->position,
        this->position - this->direction,
        this->upVector
    );
}

glm::mat4 Camera::getViewMatrixElevated()
{
    glm::vec3 rotationAxis = this->getLeft();
    glm::mat4 cameraTransform = glm::rotate(glm::mat4(1.0f), glm::radians(this->elevationAngle), rotationAxis);

    glm::vec3 newUpVector = glm::vec3(cameraTransform * glm::vec4(this->upVector, 0.0f));
    glm::vec3 cameraViewDirection = glm::vec3(cameraTransform * glm::vec4(this->direction, 0.0f));

    return this->getViewMatrix(cameraViewDirection, newUpVector);
}

glm::mat4 Camera::getViewMatrix(glm::vec3 direction, glm::vec3 upVector) const {
    return glm::lookAt(
        this->position,
        this->position - direction,
        upVector
    );
}

glm::mat4 Camera::getProjectionMatrix() const {
    return this->projectionMatrix;
}

glm::vec3 Camera::getPosition() {
    return this->position;
}

void Camera::setProjectionMatrixRatio(float ratio)
{
    this->projectionMatrix = glm::perspective(glm::radians(45.0f), ratio, 1.0f, 150.0f);
}

void Camera::setProjectionMatrixRatio(float width, float height)
{
    this->projectionMatrix = glm::perspective(glm::radians(45.0f), width / height, 1.0f, 150.0f);
}
