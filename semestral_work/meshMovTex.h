#pragma once

#include "object.h"

class MeshMovTex : public ObjectInstance
{
public:

	MeshMovTex(ShaderProgram* shdrPrg = nullptr);
	MeshMovTex(ShaderProgram* shdrPrg, const std::string& fileName);
	~MeshMovTex();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	void setScrollSpeed(glm::vec2 newSpeed);
private:
	glm::vec2 scrollSpeed = glm::vec2(0.2f, 0.0f);
	float elapsedTime = 0.0f;

	bool loadSingleMesh(const std::string& fileName, ShaderProgram* shader, ObjectGeometry** geometry);
};
