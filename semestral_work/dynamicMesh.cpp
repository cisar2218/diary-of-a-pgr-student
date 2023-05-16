#pragma once

#include "dynamicMesh.h"


void DynamicMesh::update(float elapsedTime, const glm::mat4* parentModelMatrix)
{
	this->elapsedTime = elapsedTime;
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void DynamicMesh::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {

		glUseProgram(shaderProgram->program);

		bindCommonUniforms(viewMatrix, projectionMatrix);

		// dynamic mesh specific
		glUniform1f(shaderProgram->locations.elapsedTime, this->elapsedTime);

		glBindVertexArray(geometry->vertexArrayObject);
		glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else {
		std::cerr << "SingleMesh::draw(): Can't draw, mesh not initialized properly!" << std::endl;
	}
}
