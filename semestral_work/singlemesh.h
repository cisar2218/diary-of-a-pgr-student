#pragma once

#include "object.h"

class SingleMesh : public ObjectInstance
{
public:

	SingleMesh(ShaderProgram* shdrPrg = nullptr);
	SingleMesh(ShaderProgram* shdrPrg, const std::string& fileName);
	~SingleMesh();

protected:
	bool loadSingleMesh(const std::string& fileName, ShaderProgram* shader, ObjectGeometry** geometry);
};