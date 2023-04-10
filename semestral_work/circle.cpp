#include "circle.h"

// Constructor with position and radius (default speed is 0)
CircleFlat::CircleFlat(glm::vec3 position, float radius)
    : CircleFlat(position, radius, 0.0f) {
}

// Constructor with position, radius, and speed
CircleFlat::CircleFlat(glm::vec3 position, float radius, float speed)
    : position(position), radius(radius), speed(speed), angle(0.0f), currentTime(0.0f) {
}

CircleFlat::~CircleFlat()
{
}

glm::vec3 CircleFlat::update(float elapsedTime)
{
    float deltaTime = elapsedTime - this->currentTime;

    this->angle += deltaTime * this->speed;

    float x = this->position.x + this->radius * cos(this->angle);
    float z = this->position.z + this->radius * sin(this->angle);
    
    this->currentTime = elapsedTime;
    
    return glm::vec3(x, this->position.y, z);
}
