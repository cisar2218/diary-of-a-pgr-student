#pragma once

#include "pgr.h"

/**
 * \brief Shader program related stuff (id, locations, ...).
 */
typedef struct _ShaderProgram {
	/// identifier for the shader program
	GLuint program;

	bool initialized;

	/**
	  * \brief Indices of the vertex shader inputs (locations)
	  */
	struct {
		// vertex attributes locations
		GLint position;
		GLint normal;
		GLint textureCoord;
		
		// uniforms locations
		// -> matrixes
		GLint PVM;
		GLint Vmatrix;
		GLint Mmatrix;
		GLint Nmatrix;

		// -> material
		GLint materialAmbient;
		GLint materialDiffuse;
		GLint materialSpecular;
		GLint materialShininess;

		// -> texture
		GLint textureEnabled;
		GLint textureSampler;
		GLint frame;
		GLint scrollSpeed;
		GLint elapsedTime;
	} locations;

	// ...

	_ShaderProgram() : program(0), initialized(false) {
		locations.position = -1;
		locations.normal = -1;
		locations.textureCoord = -1;

		locations.PVM = -1;
		locations.materialAmbient = -1;
		locations.materialDiffuse = -1;
		locations.materialSpecular = -1;
		locations.materialShininess = -1;
		locations.textureEnabled = -1;
	}

} ShaderProgram;

/**
 * \brief Geometry of an object (vertices, triangles).
 */
typedef struct _ObjectGeometry {
	GLuint        vertexBufferObject;   ///< identifier for the vertex buffer object
	GLuint        elementBufferObject;  ///< identifier for the element buffer object
	GLuint        vertexArrayObject;    ///< identifier for the vertex array object
	unsigned int  numTriangles;         ///< number of triangles in the mesh
} ObjectGeometry;

typedef struct _ObjectMaterial {
	glm::vec3        ambient;
	glm::vec3        diffuse;
	glm::vec3        specular;
	float			 shininess;
} ObjectMaterial;

typedef struct _ObjectTexture {
	bool enabled;
	GLint texture;
} ObjectTexture;

class ObjectInstance;
/**
 * \brief Linear representation of the scene objects.  The objects themselves may represent the subtrees.
 */
typedef std::vector<ObjectInstance*> ObjectList;

class ObjectInstance {

protected:

	ObjectGeometry* geometry;
	ObjectMaterial* material;
	ObjectTexture* texture;
	glm::mat4		localModelMatrix;
	glm::mat4		globalModelMatrix;

	// dynamic objects
	// glm::vec3 direction;
	// float     speed;
	// ...

	ShaderProgram* shaderProgram;

	ObjectList children;

	bool initialized = false;  ///< object has the shader with defined locations
public:

	/**
	 * \brief ObjectInstance constructor. Takes a pointer to the shader and must create object resources (VBO and VAO)
	 * \param shdrPrg pointer to the shader program for rendering objects data
	 */
	ObjectInstance(ShaderProgram* shdrPrg = nullptr) : geometry(nullptr), shaderProgram(shdrPrg) {}
	~ObjectInstance() {}

	virtual void setPosition(float x, float y, float z) {
		this->localModelMatrix[3] = glm::vec4(x, y, z, 1.0f);
	}

	virtual void setXPosition(float x) {
		this->localModelMatrix[3][0] = x;
	}

	virtual void setYPosition(float y) {
		this->localModelMatrix[3][1] = y;
	}

	virtual void setZPosition(float z) {
		this->localModelMatrix[3][2] = z;
	}

	virtual glm::vec3 getPosition() {
		return glm::vec3(
			this->localModelMatrix[3].x,
			this->localModelMatrix[3].y,
			this->localModelMatrix[3].z
		);
	}

	virtual void scale(float scaleRatioX, float scaleRatioY, float scaleRatioZ) {
		this->localModelMatrix = glm::scale(localModelMatrix, glm::vec3(scaleRatioX, scaleRatioY, scaleRatioZ));
	}

	virtual void scale(float scaleRatio) {
		this->localModelMatrix = glm::scale(localModelMatrix, glm::vec3(scaleRatio));
	}

	/**
	* \brief Recalculates the global matrix and updates all children.
	*   Derived classes should also call this method (using ObjectInstance::update()).
	* \param elapsedTime time value in seconds, such as 0.001*glutGet(GLUT_ELAPSED_TIME) (conversion milliseconds => seconds)
	* \param parentModelMatrix parent transformation in the scene-graph subtree
	*/
	virtual void update(const float elapsedTime, const glm::mat4* parentModelMatrix) {

		// if we have parent, multiply parent's matrix with ours
		if (parentModelMatrix != nullptr)
			globalModelMatrix = *parentModelMatrix * localModelMatrix;
		else
			globalModelMatrix = localModelMatrix;

		// update all children
		for (ObjectInstance* child : children) {
			if (child != nullptr)
				child->update(elapsedTime, &globalModelMatrix);
		}
	}

	/**
	 * \brief Draw instance geometry and calls the draw() on child nodes.
	 */
	virtual void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
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

	const glm::mat4 getModelRotationMatrix() {
		const glm::mat4 modelRotationMatrix = glm::mat4(
			globalModelMatrix[0],
			globalModelMatrix[1],
			globalModelMatrix[2],
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);
		return glm::transpose(glm::inverse(modelRotationMatrix));
	}

	void rotateYAxis(float angleInDegrees) {
		float angleInRadians = glm::radians(angleInDegrees);
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleInRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		this->localModelMatrix = this->localModelMatrix * rotationMatrix;
	}

	void rotateXAxis(float angleInDegrees) {
		float angleInRadians = glm::radians(angleInDegrees);
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleInRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		this->localModelMatrix = this->localModelMatrix * rotationMatrix;
	}

	void rotateZAxis(float angleInDegrees) {
		float angleInRadians = glm::radians(angleInDegrees);
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleInRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		this->localModelMatrix = this->localModelMatrix * rotationMatrix;
	}

	virtual void setTexture(GLint textureToSet) {
		texture->enabled = true;
		texture->texture = textureToSet;
	}

	virtual void setMaterial(ObjectMaterial* materialToSet) {
		material = materialToSet;
	}

	virtual void setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
		material = new ObjectMaterial;
		material->ambient = ambient;
		material->diffuse = diffuse;
		material->specular = specular;
		material->shininess = shininess;
	}
};
