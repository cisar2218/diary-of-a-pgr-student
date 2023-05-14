#pragma once

#include <iostream>
#include "singlemesh.h"

using namespace std;

class DynamicMesh : public SingleMesh
{
public:
	using SingleMesh::SingleMesh;

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
private:
	float elapsedTime = 0.0f;
};