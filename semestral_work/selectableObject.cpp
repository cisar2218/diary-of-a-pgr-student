#include <iostream>
#include "selectableObject.h"

void SelectableObject::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	

	// propagate the update to children
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}


void SelectableObject::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		// uniform material
		glUniform3fv(shaderProgram->locations.materialAmbient, 1, glm::value_ptr(material->ambient));
		glUniform3fv(shaderProgram->locations.materialDiffuse, 1, glm::value_ptr(material->diffuse));
		glUniform3fv(shaderProgram->locations.materialSpecular, 1, glm::value_ptr(material->specular));
		glUniform1f(shaderProgram->locations.materialShininess, material->shininess);

		// texture
		if (texture->enabled) {
			glActiveTexture(GL_TEXTURE0); // “logical” texture unit
			glBindTexture(GL_TEXTURE_2D, texture->texture);
			glUniform1i(shaderProgram->locations.textureSampler, 0);
		}
		glUniform1i(shaderProgram->locations.textureEnabled, texture->enabled);

		// uniform PVM
		glUniformMatrix4fv(shaderProgram->locations.PVM, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * globalModelMatrix));
		// uniform V matrix, M matrix, N matrix
		glm::mat4 Nmatrix = getModelRotationMatrix();
		glUniformMatrix4fv(shaderProgram->locations.Nmatrix, 1, GL_FALSE, glm::value_ptr(Nmatrix));
		glUniformMatrix4fv(shaderProgram->locations.Vmatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(shaderProgram->locations.Mmatrix, 1, GL_FALSE, glm::value_ptr(globalModelMatrix));

		glBindVertexArray(geometry->vertexArrayObject);
		glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else {
		std::cerr << "SingleMesh::draw(): Can't draw, mesh not initialized properly!" << std::endl;
	}
}

void SelectableObject::setFunction(std::function<void()> func)
{
	this->storedFunction = func;
}

void SelectableObject::executeFunction()
{
	if (storedFunction) {
		storedFunction();
	}
}

void SelectableObject::setId(int newId)
{
	this->id = newId;
}

int SelectableObject::getId()
{
	return this->id;
}
