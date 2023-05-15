#include <iostream>
#include "movingObject.h"

void MovingObject::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	float radius = 15.0f;
	float speed = 1.0f;
	float angle = speed * elapsedTime;

	float x = radius * cos(angle);
	float z = radius * sin(angle);

	glm::vec3 currDir = glm::vec3(-sin(angle), 0.0f, cos(angle));
	float deltaAngle = glm::acos(glm::dot(currDir, this->direction));

	this->rotateYAxis(-glm::degrees(deltaAngle));
	this->direction = currDir;
	this->setPosition(x, 0.0f, z);

	// propagate the update to children
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

glm::vec3 MovingObject::getDirection()
{
	return this->direction;
}
