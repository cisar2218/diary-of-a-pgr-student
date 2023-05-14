#pragma once

#include "object.h"

class MeshDynTex : public ObjectInstance
{
public:

	MeshDynTex(ShaderProgram* shdrPrg = nullptr);
	MeshDynTex(ShaderProgram* shdrPrg, const std::string& fileName, const int totalFrames);
	~MeshDynTex();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
	
	void toggleEnabled();
private:
	int frame = 0;
	int numFrames = 4;
	bool loadSingleMesh(const std::string& fileName, ShaderProgram* shader, ObjectGeometry** geometry);

	bool enabled = false;
};
