#include <iostream>
#include "movingObject.h"

/**
 * \brief Moves and rotates this object on the circle path.
 *
 * Also calls ObjectInstatance::update(..) on the top.
 */
void MovingObject::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	if (startTime == -1.0f) {
		startTime = elapsedTime;
	}

	float radius = 15.0f;
	float speed = 1.0f;
	float angle = speed * (elapsedTime - startTime);

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
