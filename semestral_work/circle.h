#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class CircleFlat
{
public:
	CircleFlat(glm::vec3 position, float radius);
	CircleFlat(glm::vec3 position, float radius, float speed);
	~CircleFlat();

	glm::vec3 update(float elapsedTime);
	void setSpeed(float newSpeed);

private:
	glm::vec3 position;
	float radius;
	float speed;
	float currentTime;
	float angle;

};

