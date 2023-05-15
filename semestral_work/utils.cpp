#include "utils.h"
#include "movingObject.h"

void printErrIfNotSatisfied(const bool condition, const std::string& errMessage) {
	if (!condition) {
		cerr << errMessage << endl;
	}
}

shared_ptr<SingleMesh> initSofa(ShaderProgram* shaderProgram)
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
	return sofa;
}
