#pragma once

#include <iostream>
#include "singlemesh.h"
#include <functional>

using namespace std;

class SelectableObject : public SingleMesh
{
public:
	using SingleMesh::SingleMesh;

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	
	void setFunction(std::function<void()> func);
	void executeFunction();

	void setId(int newId);
	int getId();
private:
	std::function<void()> storedFunction;
	int id = -1;
};
