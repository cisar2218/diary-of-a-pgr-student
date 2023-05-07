#include <iostream>
#include "singlemesh.h"

using namespace std;


void SingleMesh::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	// instance specific stuff
	float rotationSpeed = 0.1f;
	float angle = elapsedTime * rotationSpeed; // rotationSpeed is in radians per second

	glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
	glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), angle, xAxis);

	glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
	glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), angle, yAxis);

	glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
	glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1.0f), angle, zAxis);

	this->localModelMatrix = rotationMatrixX;

	// propagate the update to children
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void SingleMesh::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		material = new ObjectMaterial;
		material->ambient = glm::vec3(0.0f, 0.0f, 1.0f);
		material->diffuse = glm::vec3(1.0f, 0.7f, 0.0f);
		material->specular = glm::vec3(0.0f, 0.0f, 0.0f);
		material->shininess = 64.0f;


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
		glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else {
		std::cerr << "SingleMesh::draw(): Can't draw, mesh not initialized properly!" << std::endl;
	}
}


/** Load one mesh using assimp library (vertices only, for more attributes see method extended version in Asteroids)
 * \param fileName [in] file to open/load
 * \param shader [in] vao will connect loaded data to shader
 * \param geometry
 */
bool SingleMesh::loadSingleMesh(const std::string& fileName, ShaderProgram* shader, ObjectGeometry** geometry) {
	Assimp::Importer importer;

	// unitize object in size (scale the model to fit into (-1..1)^3)
	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

	// load asset from the file - you can play with various processing steps
	const aiScene* scn = importer.ReadFile(fileName.c_str(), 0
		| aiProcess_Triangulate             // triangulate polygons (if any)
		| aiProcess_PreTransformVertices    // transforms scene hierarchy into one root with geometry-leafs only, for more see Doc
		| aiProcess_GenSmoothNormals        // calculate normals per vertex
		| aiProcess_JoinIdenticalVertices);

	// abort if the loader fails
	if (scn == NULL) {
		std::cerr << "SingleMesh::loadSingleMesh(): assimp error - " << importer.GetErrorString() << std::endl;
		*geometry = NULL;
		return false;
	}

	// some formats store whole scene (multiple meshes and materials, lights, cameras, ...) in one file, we cannot handle that in our simplified example
	if (scn->mNumMeshes != 1) {
		std::cerr << "SingleMesh::loadSingleMesh(): this simplified loader can only process files with only one mesh" << std::endl;
		*geometry = NULL;
		return false;
	}

	// in this phase we know we have one mesh in our loaded scene, we can directly copy its data to OpenGL ...
	const aiMesh* mesh = scn->mMeshes[0];

	*geometry = new ObjectGeometry;

	// vertex buffer object, store all vertex positions
	glGenBuffers(1, &((*geometry)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);

	unsigned vertCount = 6 * mesh->mNumVertices;
	unsigned vertSize = vertCount * sizeof(float);
	float* vertexes = new float[vertCount];

	for (unsigned int v = 0; v < mesh->mNumVertices; ++v) {
		vertexes[v * 6 + 0] = mesh->mVertices[v].x;
		vertexes[v * 6 + 1] = mesh->mVertices[v].y;
		vertexes[v * 6 + 2] = mesh->mVertices[v].z;

		vertexes[v * 6 + 3] = mesh->mNormals[v].x;
		vertexes[v * 6 + 4] = mesh->mNormals[v].y;
		vertexes[v * 6 + 5] = mesh->mNormals[v].z;
	}

	glBufferData(GL_ARRAY_BUFFER, vertSize, vertexes, GL_STATIC_DRAW);     // allocate memory for vertices
	//glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * mesh->mNumVertices, mesh->mVertices); // store all vertices

	//if (mesh->HasNormals()) {
	//	glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * mesh->mNumVertices, 3 * sizeof(float) * mesh->mNumVertices, mesh->mNormals); // store all vertices
	//}


	// copy all mesh faces into one big array (assimp supports faces with ordinary number of vertices, we use only 3 -> triangles)
	unsigned int* indices = new unsigned int[mesh->mNumFaces * 3];
	
	for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
		indices[f * 3 + 0] = mesh->mFaces[f].mIndices[0];
		indices[f * 3 + 1] = mesh->mFaces[f].mIndices[1];
		indices[f * 3 + 2] = mesh->mFaces[f].mIndices[2];
	}
	
	// copy our temporary index array to OpenGL and free the array
	glGenBuffers(1, &((*geometry)->elementBufferObject));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int) * mesh->mNumFaces, indices, GL_STATIC_DRAW);

	delete[] indices;

	// copy the material info to MeshGeometry structure
	const aiMaterial* mat = scn->mMaterials[mesh->mMaterialIndex];
	aiColor4D color;
	aiString name;
	aiReturn retValue = AI_SUCCESS;

	// get returns: aiReturn_SUCCESS 0 | aiReturn_FAILURE -1 | aiReturn_OUTOFMEMORY -3
	mat->Get(AI_MATKEY_NAME, name); // may be "" after the input mesh processing, must be aiString type!

	if ((retValue = aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color)) != AI_SUCCESS)
		color = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	

	glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
	glBindVertexArray((*geometry)->vertexArrayObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject); // bind our element array buffer (indices) to vao
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);

	bool validInit = false;

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1)) {

		glEnableVertexAttribArray(shader->locations.position);
		glVertexAttribPointer(shader->locations.position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		if (mesh->HasNormals()) {
			glEnableVertexAttribArray(shader->locations.normal);
			glVertexAttribPointer(shader->locations.normal, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		}

		CHECK_GL_ERROR();

		validInit = true;
	}

	glBindVertexArray(0);

	(*geometry)->numTriangles = mesh->mNumFaces;

	return validInit;
}

SingleMesh::SingleMesh(ShaderProgram* shdrPrg, const std::string& fileName) : ObjectInstance(shdrPrg), initialized(false)
{
	if (!loadSingleMesh(fileName, shdrPrg, &geometry)) {
		if (geometry == nullptr) {
			std::cerr << "SingleMesh::SingleMesh(): geometry not initialized!" << std::endl;
		}
		else {
			std::cerr << "SingleMesh::SingleMesh(): shaderProgram struct not initialized!" << std::endl;
		}
	}
	else {
		if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.PVM != -1)) {
			initialized = true;
		}
		else {
			std::cerr << "SingleMesh::SingleMesh(): shaderProgram struct not initialized!" << std::endl;
		}
	}
}

SingleMesh::SingleMesh(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	const char* MODEL_FILE_NAME = "models/floorcube.dae";

	if (!loadSingleMesh(MODEL_FILE_NAME, shdrPrg, &geometry)) {
		if (geometry == nullptr) {
			std::cerr << "SingleMesh::SingleMesh(): geometry not initialized!" << std::endl;
		}
		else {
			std::cerr << "SingleMesh::SingleMesh(): shaderProgram struct not initialized!" << std::endl;
		}
	}
	else {
		if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.PVM != -1)) {
			initialized = true;
		}
		else {
			std::cerr << "SingleMesh::SingleMesh(): shaderProgram struct not initialized!" << std::endl;
		}
	}
}

SingleMesh::~SingleMesh() {

	if (geometry != nullptr) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));

		delete geometry;
		geometry = nullptr;
	}

	initialized = false;
}
