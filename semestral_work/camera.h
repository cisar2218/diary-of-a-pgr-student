#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 upVector;
    
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 1.0f, 150.0f);
public:
    Camera(); // default params
    Camera(const glm::vec3& startPosition, const glm::vec3& startDirection, const glm::vec3& startOrientation);
    void rotate(float angle, const glm::vec3& axis);

    void move(glm::vec3 moveVector);

    void moveRight(float movement);
    void moveLeft(float movement);

    void moveForward(float movement);

    void moveBackward(float movement);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
};