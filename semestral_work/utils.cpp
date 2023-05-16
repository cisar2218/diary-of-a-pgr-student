#include "utils.h"
#include "movingObject.h"

/**
 * \brief Print error message when condition is false.
 *
 * \param condition
 * \param error message to be print if condition is false
 */
void printErrIfNotSatisfied(const bool condition, const std::string& errMessage) {
	if (!condition) {
		cerr << errMessage << endl;
	}
}

shared_ptr<SingleMesh> initSofa(ShaderProgram* shaderProgram, GLint texture)
{
	auto sofa = make_shared<SingleMesh>(shaderProgram, "models/sofa.obj");
	
	// transforms
	sofa->rotateYAxis(-30.0f);
	sofa->scale(1.2f);
	sofa->setYPosition(-0.5f);
	sofa->setXPosition(-1.2f);
	sofa->setZPosition(1.2f);
	
	// red sofa material
	sofa->setMaterial(
		glm::vec3(0.15f, 0.01f, 0.01f),
		glm::vec3(0.8f, 0.1f, 0.1f),
		glm::vec3(0.5f, 0.1f, 0.1f),
		25.0f
	);

	if (texture != -1) {
		sofa->setTexture(texture);
	}
	return sofa;
}

shared_ptr<SingleMesh> initTower(ShaderProgram* shaderProgram, GLint texture)
{
	auto tower = make_shared<SingleMesh>(shaderProgram, "models/tower.fbx");

	//transform
	tower->rotateYAxis(180.0f);
	tower->setYPosition(-10.25f);
	tower->scale(20.0f);

	if (texture != -1) {
		tower->setTexture(texture);
	}
	return tower;
}

shared_ptr<SingleMesh> initRailing(ShaderProgram* shaderProgram, GLint texture)
{
	auto railing = make_shared<SingleMesh>(shaderProgram, "models/railing.fbx");

	railing->rotateYAxis(180.0f);
	railing->setXPosition(-1.0f);
	railing->setYPosition(-0.8f);
	railing->scale(5.0f, 2.0f, 5.0f);

	if (texture != -1) {
		railing->setTexture(texture);
	}
	return railing;
}

shared_ptr<DynamicMesh> initPlane(ShaderProgram* shaderProgram, GLint texture)
{
	auto plane = make_shared<DynamicMesh>(shaderProgram, "models/planeSubdivided.fbx");

	plane->scale(10.0f);
	plane->setPosition(0.0f, -10.0f, 30.0f);

	if (texture != -1) {
		plane->setTexture(texture);
	}
	return plane;
}

shared_ptr<MeshMovTex> initMovTexObj(ShaderProgram* shaderProgram, GLint texture)
{
	auto movTex = make_shared<MeshMovTex>(shaderProgram, "models/cubeDynamicTexture.fbx");
	movTex->rotateYAxis(90.0f);
	movTex->scale(2.0f, 0.2f, 0.5f);
	movTex->setPosition(3.5f, -0.4f, 0.0f);

	auto screenMaterial = new ObjectMaterial;
	screenMaterial->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	screenMaterial->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	screenMaterial->specular = glm::vec3(0.2f, 0.2f, 0.2f);
	screenMaterial->shininess = 10.0f;

	movTex->setMaterial(screenMaterial);
	if (texture != -1) {
		movTex->setTexture(texture);
	}
	return movTex;
}
