#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "parameters.h"
#include <iostream>
using namespace std;

/**
 * \brief Default constructor
 *
 * Initializes a camera at the origin, pointing along the z-axis, with the y-axis as the up direction.
 */
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

/**
 * \brief Copy camera properties from another camera
 *
 * \param other The camera to copy properties from
 */
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

/**
 * \brief Rotate the camera horizontally
 *
 * \param angle The angle to rotate, in radians
 */
void Camera::rotateHorizontal(float angle)
{
    glm::mat2x2 rotMat = glm::mat2(
        glm::vec2(cos(angle), sin(angle)), 
        glm::vec2(-sin(angle), cos(angle))
    );
    glm::vec2 rotated2dVec = rotMat * glm::vec2(this->direction.x, this->direction.z);
    this->direction = glm::normalize(glm::vec3(rotated2dVec.x, 0.0f ,rotated2dVec.y));
}

/**
 * \brief Move the camera by specified step
 *
 * \param step to move (size and direction)
 */
void Camera::move(glm::vec3 movement) {
    this->position += movement;
}

/**
 * \brief Move the camera to the right
 */
void Camera::moveRight(float movement) {
    glm::vec3 left = glm::cross(this->direction, this->upVector);
    this->move(left * -movement);
}

/**
 * \brief Move the camera to the left
 */
void Camera::moveLeft(float movement)
{
    glm::vec3 left = glm::cross(this->direction, this->upVector);
    this->move(left * movement);
}

/**
 * \brief Move camera in current direction by specified step size
 */
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

/**
 * \brief Get the camera's front direction
 *
 * \return The camera's front direction
 */
glm::vec3 Camera::getFront() {
    return this->direction;
}

glm::vec3 Camera::getLeft() {
    return glm::cross(this->direction, this->upVector);
}

/**
 * \brief Increase the camera's elevation angle
 *
 * \param angle The angle to increase by, in radians
 */
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

/**
 * \brief Get the camera's view matrix
 *
 * \return The camera's view matrix
 */
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(
        this->position,
        this->position - this->direction,
        this->upVector
    );
}

/**
 * \brief Get the camera's view matrix
 *
 * \return The camera's view matrix that include elevation
 */
glm::mat4 Camera::getViewMatrixElevated()
{
    glm::vec3 rotationAxis = this->getLeft();
    glm::mat4 cameraTransform = glm::rotate(glm::mat4(1.0f), glm::radians(this->elevationAngle), rotationAxis);

    glm::vec3 newUpVector = glm::vec3(cameraTransform * glm::vec4(this->upVector, 0.0f));
    glm::vec3 cameraViewDirection = glm::vec3(cameraTransform * glm::vec4(this->direction, 0.0f));

    return this->getViewMatrixElevated(cameraViewDirection, newUpVector);
}

/**
 * \brief Get the camera's view matrix
 * 
 * \param direction of object
 * \param upvector of object
 *
 * \return The camera's view matrix that include elevation
 */
glm::mat4 Camera::getViewMatrixElevated(glm::vec3 direction, glm::vec3 upVector) const {
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

/**
 * \brief Set the camera's projection matrix based on a given aspect ratio.
 *
 * \param width and height of the vieport
 * \param ratio The aspect ratio (width divided by height)
 */
void Camera::setProjectionMatrixRatio(float ratio)
{
    this->projectionMatrix = glm::perspective(glm::radians(45.0f), ratio, 1.0f, 150.0f);
}

/**
 * \brief Set the camera's projection matrix based on a given aspect ratio.
 *
 * \param width of the viewport
 * \param height of the viewport
 */
void Camera::setProjectionMatrixRatio(float width, float height)
{
    this->projectionMatrix = glm::perspective(glm::radians(45.0f), width / height, 1.0f, 150.0f);
}
