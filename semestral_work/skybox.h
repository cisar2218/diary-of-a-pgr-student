#pragma once

#include "object.h"

class Skybox : public ObjectInstance
{
public:

	Skybox(ShaderProgram* shdrPrg = nullptr);
	Skybox(ShaderProgram* shdrPrg, const std::string& fileName);
	~Skybox();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	void setPosition(glm::vec3 newPosition);
private:
	glm::vec3 position = glm::vec3(0.0f);

	bool loadSingleMesh(const std::string& fileName, ShaderProgram* shader, ObjectGeometry** geometry);

	bool initialized;  ///< object has the shader with defined locations
};
