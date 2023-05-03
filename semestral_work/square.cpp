#include <iostream>
#include "square.h"


void Square::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	// instance specific stuff
// ======== BEGIN OF SOLUTION - TASK 1_0-3 ======== //
// ========  END OF SOLUTION - TASK 1_0-3  ======== //

	// propagate the update to children
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Square::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		glUniformMatrix4fv(shaderProgram->locations.PVM, 1, GL_FALSE, glm::value_ptr(globalModelMatrix));

		glBindVertexArray(geometry->vertexArrayObject);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
	else {
		std::cerr << "Triangle::draw(): Can't draw, triangle not initialized properly!" << std::endl;
	}
}

Square::Square(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	geometry = new ObjectGeometry;

	static constexpr float vertices[] = {
	  0.0f,  0.5f,
	 -0.5f, -0.5f,
	  0.5f, -0.5f,
	  0.0f, -1.0f,
	};

	geometry->numTriangles = 1;
	geometry->elementBufferObject = 0;

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVM != -1)) {
		glEnableVertexAttribArray(shaderProgram->locations.position);
		glVertexAttribPointer(shaderProgram->locations.position, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		initialized = true;
	}
	else {
		std::cerr << "Triangle::Triangle(): shaderProgram struct not initialized!" << std::endl;
	}
}

Square::~Square() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}
