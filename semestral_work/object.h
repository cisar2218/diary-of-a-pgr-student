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
	  * \brief Indices of the vertex shader inputs (locations).
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

		// -> light
		GLint lightAmbient;
		GLint lightDiffuse;
		GLint lightSpecular;
		GLint lightPosition;
		GLint lightDirection;
	} locations;

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

		locations.lightAmbient = -1;
		locations.lightDiffuse = -1;
		locations.lightSpecular = -1;
		locations.lightPosition = -1;
		locations.lightDirection = -1;

		locations.elapsedTime = -1;
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

/**
 * \brief Material of the object (Phong reflection model).
 */
typedef struct _ObjectMaterial {
	glm::vec3        ambient;
	glm::vec3        diffuse;
	glm::vec3        specular;
	float			 shininess;
} ObjectMaterial;

/**
 * \brief Texture of the object.
 */
typedef struct _ObjectTexture {
	bool enabled;                       ///< if object has texture
	GLint texture;
} ObjectTexture;

class ObjectInstance;
/**
 * \brief Linear representation of the scene objects.  The objects themselves may represent the subtrees.
 */
typedef std::vector<std::shared_ptr<ObjectInstance>> ObjectList;

class ObjectInstance {

protected:

	ObjectGeometry* geometry;
	ObjectMaterial* material;
	ObjectTexture* texture;

	glm::mat4		localModelMatrix;
	glm::mat4		globalModelMatrix;

	ShaderProgram* shaderProgram;

	ObjectList children;

	glm::vec3 lightColorCombined;
	const glm::vec3 lightColor1 = glm::vec3(1.0f, 0.8f, 0.5f);  // Red
	const glm::vec3 lightColor2 = glm::vec3(0.8f, 1.0f, 0.5f);  // Blue

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

	virtual void setPosition(glm::vec3 newPosition) {
		this->localModelMatrix[3] = glm::vec4(newPosition, 1.0f);
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

	/**
	* \brief scales object by given ratios
	* \param ratio for X scale
	* \param ratio for Y scale
	* \param ratio for Z scale
	*/
	virtual void scale(float scaleRatioX, float scaleRatioY, float scaleRatioZ) {
		this->localModelMatrix = glm::scale(localModelMatrix, glm::vec3(scaleRatioX, scaleRatioY, scaleRatioZ));
	}

	/**
	* \brief scale object by given ratio
	* \param uniform ratio scale
	*/
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

		// update light color
		this->lightColorCombined = glm::mix(this->lightColor1, this->lightColor2, glm::sin(elapsedTime) / 2.0f + 1);

		// if we have parent, multiply parent's matrix with ours
		if (parentModelMatrix != nullptr)
			globalModelMatrix = *parentModelMatrix * localModelMatrix;
		else
			globalModelMatrix = localModelMatrix;

		// update all children
		for (auto child : children) {
			if (child != nullptr)
				child->update(elapsedTime, &globalModelMatrix);
		}
	}

	/**
	* \brief Bind uniforms used in common shader.
	*   
	* \param view matrix of camera
	* \param projection matrix of camera
	*/
	virtual void bindCommonUniforms(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
		// uniform material
		glUniform3fv(shaderProgram->locations.materialAmbient, 1, glm::value_ptr(material->ambient));
		glUniform3fv(shaderProgram->locations.materialDiffuse, 1, glm::value_ptr(material->diffuse));
		glUniform3fv(shaderProgram->locations.materialSpecular, 1, glm::value_ptr(material->specular));
		glUniform1f(shaderProgram->locations.materialShininess, material->shininess);

		// texture
		if (texture->enabled) {
			glActiveTexture(GL_TEXTURE0); // �logical� texture unit
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

		// uniform light
		glUniform3fv(shaderProgram->locations.lightAmbient, 1, glm::value_ptr(glm::vec3(0.0f)));
		glUniform3fv(shaderProgram->locations.lightDiffuse, 1, glm::value_ptr(this->lightColorCombined));
		glUniform3fv(shaderProgram->locations.lightSpecular, 1, glm::value_ptr(glm::vec3(1.0f)));
		glUniform3fv(shaderProgram->locations.lightPosition, 1, glm::value_ptr(glm::vec3(-3.0f, 2.0f, -2.5f)));
		glUniform3fv(shaderProgram->locations.lightDirection, 1, glm::value_ptr(glm::vec3(0.0f, -1.0f, 0.0f)));
	}

	/**
	 * \brief Draw instance geometry and calls the draw() on child nodes.
	 */
	virtual void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
		if (initialized && (shaderProgram != nullptr)) {

			glUseProgram(shaderProgram->program);

			bindCommonUniforms(viewMatrix, projectionMatrix);
			
			glBindVertexArray(geometry->vertexArrayObject);
			glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		else {
			std::cerr << "SingleMesh::draw(): Can't draw, mesh not initialized properly!" << std::endl;
		}
	}

	/**
	* \return rotation matrix of object
	*/
	const glm::mat4 getModelRotationMatrix() {
		const glm::mat4 modelRotationMatrix = glm::mat4(
			globalModelMatrix[0],
			globalModelMatrix[1],
			globalModelMatrix[2],
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);
		return glm::transpose(glm::inverse(modelRotationMatrix));
	}

	/**
	* \brief Rotate object around Y axis
	*/
	void rotateYAxis(float angleInDegrees) {
		float angleInRadians = glm::radians(angleInDegrees);
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleInRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		this->localModelMatrix = this->localModelMatrix * rotationMatrix;
	}

	/**
	* \brief Rotate object around X axis
	*/
	void rotateXAxis(float angleInDegrees) {
		float angleInRadians = glm::radians(angleInDegrees);
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleInRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		this->localModelMatrix = this->localModelMatrix * rotationMatrix;
	}

	/**
	* \brief Rotate object around Z axis
	*/
	void rotateZAxis(float angleInDegrees) {
		float angleInRadians = glm::radians(angleInDegrees);
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleInRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		this->localModelMatrix = this->localModelMatrix * rotationMatrix;
	}

	/**
	* \brief Bind given texture to this object
	* 
	* \param valid texture created e.g by pgr::createTexture(...);
	*/
	virtual void setTexture(GLint textureToSet) {
		texture->enabled = true;
		texture->texture = textureToSet;
	}

	/**
	* \brief Sets given material to this object (Phong reflection model)
	*/
	virtual void setMaterial(ObjectMaterial* materialToSet) {
		material = materialToSet;
	}

	/**
	* \brief Sets given properties to material of this object (Phong reflection model)
	* 
	* \param ambient color
	* \param diffuse color
	* \param specular color
	* \param shininess
	*/
	virtual void setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
		material = new ObjectMaterial;
		material->ambient = ambient;
		material->diffuse = diffuse;
		material->specular = specular;
		material->shininess = shininess;
	}
};
