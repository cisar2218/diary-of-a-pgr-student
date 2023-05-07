#include <iostream>
#include "sphere.h"

const unsigned sphereTriangles[] = {
  0, 1, 2,
  3, 4, 5,
  6, 7, 8,
  9, 10, 11,
  12, 13, 14,
  15, 16, 17,
  18, 19, 20,
  21, 22, 23,
  24, 25, 26,
  27, 28, 29,
  30, 31, 32,
  33, 34, 35,
  36, 37, 38,
  39, 40, 41,
  42, 43, 44,
  45, 46, 47,
  48, 49, 50,
  51, 52, 53,
  54, 55, 56,
  57, 58, 59,
  60, 61, 62,
  63, 64, 65,
  66, 67, 68,
  69, 70, 71,
  72, 73, 74,
  75, 76, 77,
  78, 79, 80,
  81, 82, 83,
  84, 85, 86,
  87, 88, 89,
  90, 91, 92,
  93, 94, 95,
  0, 96, 1,
  9, 97, 10,
  15, 98, 16,
  18, 99, 19,
  30, 100, 31,
  33, 101, 34,
  36, 102, 37,
  45, 103, 46,
  48, 104, 49,
  57, 105, 58,
  60, 106, 61,
  69, 107, 70,
  72, 108, 73,
  81, 109, 82,
  84, 110, 85,
  93, 111, 94,
}; // end sphereTriangles

void Sphere::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	float rotationSpeed = 1.0f;
	float angle = elapsedTime * rotationSpeed; // rotationSpeed is in radians per second

	glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
	glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), angle, xAxis);

	glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
	glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), angle, yAxis);

	glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
	glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1.0f), angle, zAxis);

	rotationMatrixY[3] = glm::vec4(this->localModelMatrix[3]);
	this->localModelMatrix = rotationMatrixY;
	// propagate the update to children
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

//const glm::mat4 Sphere::getModelRotationMatrix() {
//	const glm::mat4 modelRotationMatrix = glm::mat4(
//		globalModelMatrix[0],
//		globalModelMatrix[1],
//		globalModelMatrix[2],
//		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
//	);
//	return glm::transpose(glm::inverse(modelRotationMatrix));
//}

void Sphere::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		// uniform material
		glUniform3fv(shaderProgram->locations.materialAmbient, 1, glm::value_ptr(material->ambient));
		glUniform3fv(shaderProgram->locations.materialDiffuse, 1, glm::value_ptr(material->diffuse));
		glUniform3fv(shaderProgram->locations.materialSpecular, 1, glm::value_ptr(material->specular));
		glUniform1f(shaderProgram->locations.materialShininess, material->shininess);

		// uniform PVM
		glUniformMatrix4fv(shaderProgram->locations.PVM, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * globalModelMatrix));
		// uniform V matrix, M matrix, N matrix
		glm::mat4 Nmatrix = getModelRotationMatrix();
		glUniformMatrix4fv(shaderProgram->locations.Nmatrix, 1, GL_FALSE, glm::value_ptr(Nmatrix));
		glUniformMatrix4fv(shaderProgram->locations.Vmatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(shaderProgram->locations.Mmatrix, 1, GL_FALSE, glm::value_ptr(globalModelMatrix));
		
		glBindVertexArray(geometry->vertexArrayObject);
		glDrawElements(GL_TRIANGLES, 48*3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else {
		std::cerr << "Sphere::draw(): Can't draw, triangle not initialized properly!" << std::endl;
	}
}

void Sphere::setMaterial() {
	material = new ObjectMaterial;
	//material->ambient = glm::vec3(0.5f, 0.3f, 0.0f);
	material->ambient = glm::vec3(0.0f, 0.0f, 1.0f);
	material->diffuse = glm::vec3(1.0f, 0.7f, 0.0f);
	material->specular = glm::vec3(0.0f, 0.0f, 0.0f);
	material->shininess = 64.0f;
}

Sphere::Sphere(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	setMaterial();

	geometry = new ObjectGeometry;

	const int sphereNAttribsPerVertex = 8;
	const int sphereNVertices = 112;
	const int sphereNTriangles = 48;
	static constexpr float vertices[] = {
	  0.0f,1.0f,-4.37114e-08f,0.357407f,0.862856f,-0.357407f,0.75f,0.5f,
	  0.5f,0.5f,-0.707107f,0.357407f,0.862856f,-0.357407f,0.625f,0.25f,
	  0.0f,0.707107f,-0.707107f,0.357407f,0.862856f,-0.357407f,0.75f,0.25f,
	  0.0f,0.707107f,0.707107f,0.156558f,0.377964f,0.912487f,0.75f,0.75f,
	  -6.18172e-08f,-1.4924e-07f,1.0f,0.156558f,0.377964f,0.912487f,0.6875f,1.0f,
	  0.5f,0.5f,0.707107f,0.156558f,0.377964f,0.912487f,0.625f,0.75f,
	  0.0f,-8.74228e-08f,-1.0f,0.156558f,0.377964f,-0.912487f,0.6875f,0.0f,
	  0.0f,0.707107f,-0.707107f,0.156558f,0.377964f,-0.912487f,0.75f,0.25f,
	  0.5f,0.5f,-0.707107f,0.156558f,0.377964f,-0.912487f,0.625f,0.25f,
	  0.0f,0.707107f,0.707107f,0.357407f,0.862856f,0.357407f,0.75f,0.75f,
	  0.707107f,0.707107f,-4.37114e-08f,0.357407f,0.862856f,0.357407f,0.625f,0.5f,
	  0.0f,1.0f,-4.37114e-08f,0.357407f,0.862856f,0.357407f,0.75f,0.5f,
	  0.0f,-8.74228e-08f,-1.0f,0.377964f,0.156558f,-0.912487f,0.5625f,0.0f,
	  0.5f,0.5f,-0.707107f,0.377964f,0.156558f,-0.912487f,0.625f,0.25f,
	  0.707107f,-8.52102e-08f,-0.707107f,0.377964f,0.156558f,-0.912487f,0.5f,0.25f,
	  0.5f,0.5f,0.707107f,0.862856f,0.357407f,0.357407f,0.625f,0.75f,
	  1.0f,-5.54079e-08f,-4.37114e-08f,0.862856f,0.357407f,0.357407f,0.5f,0.5f,
	  0.707107f,0.707107f,-4.37114e-08f,0.862856f,0.357407f,0.357407f,0.625f,0.5f,
	  0.707107f,0.707107f,-4.37114e-08f,0.862856f,0.357407f,-0.357407f,0.625f,0.5f,
	  0.707107f,-8.52102e-08f,-0.707107f,0.862856f,0.357407f,-0.357407f,0.5f,0.25f,
	  0.5f,0.5f,-0.707107f,0.862856f,0.357407f,-0.357407f,0.625f,0.25f,
	  0.5f,0.5f,0.707107f,0.377964f,0.156558f,0.912487f,0.625f,0.75f,
	  -6.18172e-08f,-1.4924e-07f,1.0f,0.377964f,0.156558f,0.912487f,0.5625f,1.0f,
	  0.707107f,-8.52102e-08f,0.707107f,0.377964f,0.156558f,0.912487f,0.5f,0.75f,
	  0.707107f,-8.52102e-08f,0.707107f,0.377964f,-0.156558f,0.912487f,0.5f,0.75f,
	  -6.18172e-08f,-1.4924e-07f,1.0f,0.377964f,-0.156558f,0.912487f,0.4375f,1.0f,
	  0.5f,-0.5f,0.707107f,0.377964f,-0.156558f,0.912487f,0.375f,0.75f,
	  0.0f,-8.74228e-08f,-1.0f,0.377964f,-0.156558f,-0.912487f,0.4375f,0.0f,
	  0.707107f,-8.52102e-08f,-0.707107f,0.377964f,-0.156558f,-0.912487f,0.5f,0.25f,
	  0.5f,-0.5f,-0.707107f,0.377964f,-0.156558f,-0.912487f,0.375f,0.25f,
	  0.707107f,-8.52102e-08f,0.707107f,0.862856f,-0.357407f,0.357407f,0.5f,0.75f,
	  0.707107f,-0.707107f,-4.37114e-08f,0.862856f,-0.357407f,0.357407f,0.375f,0.5f,
	  1.0f,-5.54079e-08f,-4.37114e-08f,0.862856f,-0.357407f,0.357407f,0.5f,0.5f,
	  1.0f,-5.54079e-08f,-4.37114e-08f,0.862856f,-0.357407f,-0.357407f,0.5f,0.5f,
	  0.5f,-0.5f,-0.707107f,0.862856f,-0.357407f,-0.357407f,0.375f,0.25f,
	  0.707107f,-8.52102e-08f,-0.707107f,0.862856f,-0.357407f,-0.357407f,0.5f,0.25f,
	  0.707107f,-0.707107f,-4.37114e-08f,0.357407f,-0.862856f,-0.357407f,0.375f,0.5f,
	  3.20149e-08f,-0.707107f,-0.707107f,0.357407f,-0.862856f,-0.357407f,0.25f,0.25f,
	  0.5f,-0.5f,-0.707107f,0.357407f,-0.862856f,-0.357407f,0.375f,0.25f,
	  0.5f,-0.5f,0.707107f,0.156558f,-0.377965f,0.912487f,0.375f,0.75f,
	  -6.18172e-08f,-1.4924e-07f,1.0f,0.156558f,-0.377965f,0.912487f,0.3125f,1.0f,
	  3.20149e-08f,-0.707107f,0.707107f,0.156558f,-0.377965f,0.912487f,0.25f,0.75f,
	  0.0f,-8.74228e-08f,-1.0f,0.156558f,-0.377964f,-0.912487f,0.3125f,0.0f,
	  0.5f,-0.5f,-0.707107f,0.156558f,-0.377964f,-0.912487f,0.375f,0.25f,
	  3.20149e-08f,-0.707107f,-0.707107f,0.156558f,-0.377964f,-0.912487f,0.25f,0.25f,
	  0.5f,-0.5f,0.707107f,0.357407f,-0.862856f,0.357407f,0.375f,0.75f,
	  2.21259e-09f,-1.0f,-4.37114e-08f,0.357407f,-0.862856f,0.357407f,0.25f,0.5f,
	  0.707107f,-0.707107f,-4.37114e-08f,0.357407f,-0.862856f,0.357407f,0.375f,0.5f,
	  2.21259e-09f,-1.0f,-4.37114e-08f,-0.357407f,-0.862856f,-0.357407f,0.25f,0.5f,
	  -0.5f,-0.5f,-0.707107f,-0.357407f,-0.862856f,-0.357407f,0.125f,0.25f,
	  3.20149e-08f,-0.707107f,-0.707107f,-0.357407f,-0.862856f,-0.357407f,0.25f,0.25f,
	  3.20149e-08f,-0.707107f,0.707107f,-0.156558f,-0.377965f,0.912487f,0.25f,0.75f,
	  -6.18172e-08f,-1.4924e-07f,1.0f,-0.156558f,-0.377965f,0.912487f,0.1875f,1.0f,
	  -0.5f,-0.5f,0.707107f,-0.156558f,-0.377965f,0.912487f,0.125f,0.75f,
	  0.0f,-8.74228e-08f,-1.0f,-0.156558f,-0.377964f,-0.912487f,0.1875f,0.0f,
	  3.20149e-08f,-0.707107f,-0.707107f,-0.156558f,-0.377964f,-0.912487f,0.25f,0.25f,
	  -0.5f,-0.5f,-0.707107f,-0.156558f,-0.377964f,-0.912487f,0.125f,0.25f,
	  3.20149e-08f,-0.707107f,0.707107f,-0.357407f,-0.862856f,0.357407f,0.25f,0.75f,
	  -0.707107f,-0.707107f,-4.37114e-08f,-0.357407f,-0.862856f,0.357407f,0.125f,0.5f,
	  2.21259e-09f,-1.0f,-4.37114e-08f,-0.357407f,-0.862856f,0.357407f,0.25f,0.5f,
	  -0.5f,-0.5f,-0.707107f,-0.862856f,-0.357407f,-0.357407f,0.125f,0.25f,
	  -1.0f,-5.54079e-08f,-4.37114e-08f,-0.862856f,-0.357407f,-0.357407f,0.0f,0.5f,
	  -0.707107f,-8.52102e-08f,-0.707107f,-0.862856f,-0.357407f,-0.357407f,0.0f,0.25f,
	  -0.5f,-0.5f,0.707107f,-0.377965f,-0.156558f,0.912487f,0.125f,0.75f,
	  -6.18172e-08f,-1.4924e-07f,1.0f,-0.377965f,-0.156558f,0.912487f,0.0625f,1.0f,
	  -0.707107f,-8.52102e-08f,0.707107f,-0.377965f,-0.156558f,0.912487f,0.0f,0.75f,
	  0.0f,-8.74228e-08f,-1.0f,-0.377965f,-0.156558f,-0.912487f,0.0625f,0.0f,
	  -0.5f,-0.5f,-0.707107f,-0.377965f,-0.156558f,-0.912487f,0.125f,0.25f,
	  -0.707107f,-8.52102e-08f,-0.707107f,-0.377965f,-0.156558f,-0.912487f,0.0f,0.25f,
	  -0.5f,-0.5f,0.707107f,-0.862856f,-0.357407f,0.357407f,0.125f,0.75f,
	  -1.0f,-5.54079e-08f,-4.37114e-08f,-0.862856f,-0.357407f,0.357407f,0.0f,0.5f,
	  -0.707107f,-0.707107f,-4.37114e-08f,-0.862856f,-0.357407f,0.357407f,0.125f,0.5f,
	  -1.0f,-5.54079e-08f,-4.37114e-08f,-0.862856f,0.357407f,-0.357407f,1.0f,0.5f,
	  -0.5f,0.5f,-0.707107f,-0.862856f,0.357407f,-0.357407f,0.875f,0.25f,
	  -0.707107f,-8.52102e-08f,-0.707107f,-0.862856f,0.357407f,-0.357407f,1.0f,0.25f,
	  -0.707107f,-8.52102e-08f,0.707107f,-0.377965f,0.156558f,0.912487f,1.0f,0.75f,
	  -6.18172e-08f,-1.4924e-07f,1.0f,-0.377965f,0.156558f,0.912487f,0.9375f,1.0f,
	  -0.5f,0.5f,0.707107f,-0.377965f,0.156558f,0.912487f,0.875f,0.75f,
	  0.0f,-8.74228e-08f,-1.0f,-0.377965f,0.156558f,-0.912487f,0.9375f,0.0f,
	  -0.707107f,-8.52102e-08f,-0.707107f,-0.377965f,0.156558f,-0.912487f,1.0f,0.25f,
	  -0.5f,0.5f,-0.707107f,-0.377965f,0.156558f,-0.912487f,0.875f,0.25f,
	  -0.707107f,-8.52102e-08f,0.707107f,-0.862856f,0.357407f,0.357407f,1.0f,0.75f,
	  -0.707107f,0.707107f,-4.37114e-08f,-0.862856f,0.357407f,0.357407f,0.875f,0.5f,
	  -1.0f,-5.54079e-08f,-4.37114e-08f,-0.862856f,0.357407f,0.357407f,1.0f,0.5f,
	  -0.707107f,0.707107f,-4.37114e-08f,-0.357407f,0.862856f,-0.357407f,0.875f,0.5f,
	  0.0f,0.707107f,-0.707107f,-0.357407f,0.862856f,-0.357407f,0.75f,0.25f,
	  -0.5f,0.5f,-0.707107f,-0.357407f,0.862856f,-0.357407f,0.875f,0.25f,
	  -0.5f,0.5f,0.707107f,-0.156558f,0.377964f,0.912487f,0.875f,0.75f,
	  -6.18172e-08f,-1.4924e-07f,1.0f,-0.156558f,0.377964f,0.912487f,0.8125f,1.0f,
	  0.0f,0.707107f,0.707107f,-0.156558f,0.377964f,0.912487f,0.75f,0.75f,
	  0.0f,-8.74228e-08f,-1.0f,-0.156558f,0.377964f,-0.912487f,0.8125f,0.0f,
	  -0.5f,0.5f,-0.707107f,-0.156558f,0.377964f,-0.912487f,0.875f,0.25f,
	  0.0f,0.707107f,-0.707107f,-0.156558f,0.377964f,-0.912487f,0.75f,0.25f,
	  -0.707107f,0.707107f,-4.37114e-08f,-0.357407f,0.862856f,0.357407f,0.875f,0.5f,
	  0.0f,0.707107f,0.707107f,-0.357407f,0.862856f,0.357407f,0.75f,0.75f,
	  0.0f,1.0f,-4.37114e-08f,-0.357407f,0.862856f,0.357407f,0.75f,0.5f,
	  0.707107f,0.707107f,-4.37114e-08f,0.357407f,0.862856f,-0.357407f,0.625f,0.5f,
	  0.5f,0.5f,0.707107f,0.357407f,0.862856f,0.357407f,0.625f,0.75f,
	  0.707107f,-8.52102e-08f,0.707107f,0.862856f,0.357407f,0.357407f,0.5f,0.75f,
	  1.0f,-5.54079e-08f,-4.37114e-08f,0.862856f,0.357407f,-0.357407f,0.5f,0.5f,
	  0.5f,-0.5f,0.707107f,0.862856f,-0.357407f,0.357407f,0.375f,0.75f,
	  0.707107f,-0.707107f,-4.37114e-08f,0.862856f,-0.357407f,-0.357407f,0.375f,0.5f,
	  2.21259e-09f,-1.0f,-4.37114e-08f,0.357407f,-0.862856f,-0.357407f,0.25f,0.5f,
	  3.20149e-08f,-0.707107f,0.707107f,0.357407f,-0.862856f,0.357407f,0.25f,0.75f,
	  -0.707107f,-0.707107f,-4.37114e-08f,-0.357407f,-0.862856f,-0.357407f,0.125f,0.5f,
	  -0.5f,-0.5f,0.707107f,-0.357407f,-0.862856f,0.357407f,0.125f,0.75f,
	  -0.707107f,-0.707107f,-4.37114e-08f,-0.862856f,-0.357407f,-0.357407f,0.125f,0.5f,
	  -0.707107f,-8.52102e-08f,0.707107f,-0.862856f,-0.357407f,0.357407f,0.0f,0.75f,
	  -0.707107f,0.707107f,-4.37114e-08f,-0.862856f,0.357407f,-0.357407f,0.875f,0.5f,
	  -0.5f,0.5f,0.707107f,-0.862856f,0.357407f,0.357407f,0.875f,0.75f,
	  0.0f,1.0f,-4.37114e-08f,-0.357407f,0.862856f,-0.357407f,0.75f,0.5f,
	  -0.5f,0.5f,0.707107f,-0.357407f,0.862856f,0.357407f,0.875f,0.75f,
	}; // end sphereVertices

	geometry->numTriangles = 48;
	geometry->elementBufferObject = 0;

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereTriangles), sphereTriangles, GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVM != -1)) {
		//glEnableVertexAttribArray(shaderProgram->locations.position);
		//glVertexAttribPointer(shaderProgram->locations.position, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		initialized = true;

		// Position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sphereNAttribsPerVertex * sizeof(float), (void*)0);

		// Normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sphereNAttribsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));

		// Texture coordinate attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sphereNAttribsPerVertex * sizeof(float), (void*)(6 * sizeof(float)));

		glBindVertexArray(0);
	}
	else {
		std::cerr << "Sphere::Sphere(): shaderProgram struct not initialized!" << std::endl;
	}
}

Sphere::~Sphere() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}
