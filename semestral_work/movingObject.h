#pragma once

#include <iostream>
#include "singlemesh.h"

using namespace std;

class MovingObject : public SingleMesh
{
public:
	using SingleMesh::SingleMesh;

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;

	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override; 

	glm::vec3 getDirection();

	void toggleMovement();

private:
	bool movementEnabled = false;
	glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f);
	float lastElapsedTime = 0.0f;
};