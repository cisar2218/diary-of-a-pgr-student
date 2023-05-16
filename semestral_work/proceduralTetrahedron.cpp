#include <iostream>
#include <random>
#include <memory>
#include "proceduralTetrahedron.h"
#include "realGenerator.h"

Tetrahedron::Tetrahedron(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg)
{
	// shader not inited yet
	initialized = false;
	// texture not inited yet
	texture = new ObjectTexture;
	texture->enabled = false;

	geometry = new ObjectGeometry;

	generateVetices();
	
	const int numFaceIndexes = sizeof(faceIndexes) / sizeof(faceIndexes[0]);
	geometry->numTriangles = numFaceIndexes / 3; // 3 coord per face
	geometry->elementBufferObject = 0;

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndexes), faceIndexes, GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVM != -1)) {
		initialized = true;

		// Position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, numAttribsPerVertex * sizeof(float), (void*)0);

		// Normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, numAttribsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));

		// Texture coordinate attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, numAttribsPerVertex * sizeof(float), (void*)(6 * sizeof(float)));

		glBindVertexArray(0);
	}
	else {
		std::cerr << "Tetrahedron::Tetrahedron(): shaderProgram struct not initialized!" << std::endl;
	}
}


/**
 * \brief Generate tetrahedron vertexes
 *
 * Tetrahedron verteces are generated as random multiplications of independent vectors
 * that are set manualy to achieve reasonable shape. This way we achieve random position
 * of vertexes.
 */
void Tetrahedron::generateVetices()
{
	glm::vec2 texCoords[] = {
		glm::vec2(0.0f, 0.5),
		glm::vec2(1.0f, 0.0),
		glm::vec2(1.0f, 1.0),
	};

	float minDistance = 0.2f;
	float maxDistance = 1.2f;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(minDistance, maxDistance);

	auto randGen = std::make_shared<RealGenerator>(minDistance, maxDistance);

	for (int i = 0; i < numVertices; ++i) {
		float randomNumber = randGen->getNext();
		auto generatedPosition = randomNumber * directions[i];

		vertices[i * numAttribsPerVertex + 0] = generatedPosition.x;
		vertices[i * numAttribsPerVertex + 1] = generatedPosition.y;
		vertices[i * numAttribsPerVertex + 2] = generatedPosition.z;
		// normals
		auto normalizedPosition = glm::normalize(generatedPosition);
		vertices[i * numAttribsPerVertex + 3] = normalizedPosition.x;
		vertices[i * numAttribsPerVertex + 4] = normalizedPosition.y;
		vertices[i * numAttribsPerVertex + 5] = normalizedPosition.z;
		// tex coords
		vertices[i * numAttribsPerVertex + 6] = texCoords[i % 3].x;
		vertices[i * numAttribsPerVertex + 7] = texCoords[i % 3].y;
	}
}

Tetrahedron ::~Tetrahedron() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}