#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "movingObject.h"

class Camera {
private:
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 1.0f, 150.0f);
    float elevationAngle = 0.0f;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 upVector;

    MovingObject* refObj = nullptr;
public:
    float currentTime;

    Camera(); // default params
    Camera(const glm::vec3& startPosition);
    Camera(const glm::vec3& startPosition, const glm::vec3& startDirection);
    Camera(const glm::vec3& startPosition, const glm::vec3& startDirection, const glm::vec3& startOrientation);

    void setCameraFrom(Camera camera);

    void setRefObject(MovingObject& refObj);
    void setPositionAsReferObj();

    void rotateHorizontal(float angle);
    void increaseElevation(float angle);

    void move(glm::vec3 moveVector);
    void moveRight(float movement);
    void moveLeft(float movement);
    void moveForward(float movement);
    void moveBackward(float movement);

    void setPosition(glm::vec3 newPosition);
    void ground();

    glm::vec3 getFront();
    glm::vec3 getLeft();


    glm::mat4 getViewMatrix() const;
    glm::mat4 getViewMatrixElevated();
    glm::mat4 getViewMatrix(glm::vec3 direction, glm::vec3 upVector) const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition();

    void setProjectionMatrixRatio(float ratio);
    void setProjectionMatrixRatio(float width, float height);
};
