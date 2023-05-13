#pragma once

#include "object.h"

class Skybox : public ObjectInstance
{
public:

	Skybox(ShaderProgram* shdrPrg, const std::string& fileName);
	~Skybox();

	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:
	bool loadSingleMesh(const std::string& fileName, ShaderProgram* shader, ObjectGeometry** geometry);
};
