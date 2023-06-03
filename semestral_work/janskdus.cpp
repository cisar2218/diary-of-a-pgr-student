//----------------------------------------------------------------------------------------
/**
 * \file    janskdus.cpp : This file contains the 'main' function and callbacks.
			Program execution begins and ends there.
 * \author  Dušan Jánsky
 * \date    2022/04/02
 * \brief   Semestral work for PGR class.
 */

#include <iostream>
#include <functional>
#include "pgr.h"
#include "object.h"
#include "singlemesh.h"
#include "camera.h"
#include "parameters.h"
#include "janskdus.h"
#include "sphere.h"
#include "skybox.h"
#include "meshDynTex.h"
#include "meshMovTex.h"
#include "movingObject.h"
#include "selectableObject.h"
#include "proceduralTetrahedron.h"
#include "dynamicMesh.h"
#include "utils.h"
#include <unordered_map>
#include <glm/gtx/string_cast.hpp>
#include "realGenerator.h"

using namespace std;

enum { KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, KEY_DOWN_ARROW, KEYS_COUNT };
enum { CAMERA_FREE_IDX, CAMERA_2_IDX, CAMERA_3_IDX, CAMERA_4_MOVING_IDX, CAMERA_COUNT };

ObjectList objects;

// shared shader programs
ShaderProgram skyboxShaderProgram;
ShaderProgram commonShaderProgram;
ShaderProgram dynTexShaderProgram;
ShaderProgram movTexShaderProgram;
ShaderProgram dynamicVertShaderProgram;

Camera cameras[CAMERA_COUNT];

/**
 * \brief Global states of the scene.
 */
struct GameState {
	int windowWidth = WINDOW_WIDTH;     // set by reshape callback
	int windowHeight = WINDOW_HEIGHT;   // set by reshape callback

	int activeCamera = CAMERA_3_IDX;
	float cameraElevationAngle; // in degrees = initially 0.0f

	bool keyMap[KEYS_COUNT];    // false
	shared_ptr<Skybox> skybox = nullptr;
	bool collisionEnabled = false;
} gameState;

/**
 * \brief Set of textures used in the scene.
 */
struct Textures {
	GLint woodTexture = -1;
	GLint brickTexture = -1;
	GLint skyboxTexture = -1;
	GLint dynamicTexture = -1;
	GLint movingTexture = -1;
	GLint wallRaw = -1;
	GLint rock = -1;
} texturesInited;

// -----------------------  OpenGL stuff ---------------------------------

/**
 * \brief Scan screen for selectable objects, select selectable object if found.
 *
 * \param X coord of the mouse
 * \param Y coord of the mouse
 */
void doPicking(int x, int y) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glm::mat4 viewMatrix = cameras[gameState.activeCamera].getViewMatrixElevated();

	unordered_map<int, shared_ptr<SelectableObject>> selectableObjects;

	int id = 1;
	for (auto object : objects) {
		if (object != nullptr) {
			auto selectableObj = dynamic_pointer_cast<SelectableObject>(object);
			if (selectableObj) {
				glStencilFunc(GL_ALWAYS, id, -1);
				
				selectableObj->draw(viewMatrix, cameras[gameState.activeCamera].getProjectionMatrix());
				selectableObj->setId(id);

				selectableObjects[id] = selectableObj;
				id++;
			}
		}
	}
	glDisable(GL_STENCIL_TEST);

	// read value from the stencil buffer for one pixel under the mouse cursor
	unsigned char pixelID;
	glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &pixelID);


	if (pixelID != 0) {
		auto it = selectableObjects.find(pixelID);
		if (it != selectableObjects.end()) {
			auto selectedObject = it->second;
			std::cout << "clicked on object with ID: " << (int)pixelID << std::endl;
			selectedObject->executeFunction();
		}
		else {
			std::cout << "No SelectableObject found with ID: " << pixelID << std::endl;
		}
	}
	else {
		std::cout << "clicked on background" << pixelID << std::endl;
	}
}

/**
 * \brief Load and compile shader programs. Get attribute locations.
 */
void loadShaderPrograms()
{
	{ // sphere shaders 
		GLuint shaders[] = {
		  pgr::createShaderFromFile(GL_VERTEX_SHADER, "sphere.vert"),
		  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "sphere.frag"),
		  0
		};

		commonShaderProgram.program = pgr::createProgram(shaders);
		
		// get location of the uniform (fragment) shader attributes
		commonShaderProgram.locations.textureSampler = glGetUniformLocation(commonShaderProgram.program, "tex");
		commonShaderProgram.locations.textureEnabled = glGetUniformLocation(commonShaderProgram.program, "texEnabled");

		commonShaderProgram.locations.position = glGetAttribLocation(commonShaderProgram.program, "aPos");
		commonShaderProgram.locations.normal = glGetAttribLocation(commonShaderProgram.program, "aNormal");
		commonShaderProgram.locations.textureCoord = glGetAttribLocation(commonShaderProgram.program, "aTexCoord");

		// other attributes and uniforms
		// -> material
		commonShaderProgram.locations.materialAmbient = glGetUniformLocation(commonShaderProgram.program, "material.ambient");
		commonShaderProgram.locations.materialDiffuse = glGetUniformLocation(commonShaderProgram.program, "material.diffuse");
		commonShaderProgram.locations.materialSpecular = glGetUniformLocation(commonShaderProgram.program, "material.specular");
		commonShaderProgram.locations.materialShininess = glGetUniformLocation(commonShaderProgram.program, "material.shininess");

		// -> matrixes
		commonShaderProgram.locations.PVM = glGetUniformLocation(commonShaderProgram.program, "PVM");
		commonShaderProgram.locations.Vmatrix = glGetUniformLocation(commonShaderProgram.program, "Vmatrix");
		commonShaderProgram.locations.Mmatrix = glGetUniformLocation(commonShaderProgram.program, "Mmatrix");
		commonShaderProgram.locations.Nmatrix = glGetUniformLocation(commonShaderProgram.program, "Nmatrix");
		// -> lights
		commonShaderProgram.locations.lightAmbient = glGetUniformLocation(commonShaderProgram.program, "spotLight.ambient");
		commonShaderProgram.locations.lightDiffuse = glGetUniformLocation(commonShaderProgram.program, "spotLight.diffuse");
		commonShaderProgram.locations.lightSpecular = glGetUniformLocation(commonShaderProgram.program, "spotLight.specular");
		commonShaderProgram.locations.lightPosition = glGetUniformLocation(commonShaderProgram.program, "spotLight.position");
		commonShaderProgram.locations.lightDirection = glGetUniformLocation(commonShaderProgram.program, "spotLight.direction");

		// check for error INs
		printErrIfNotSatisfied(commonShaderProgram.locations.position != -1, "position attribLocation not found");
		printErrIfNotSatisfied(commonShaderProgram.locations.normal != -1, "normal attribLocation not found");
		printErrIfNotSatisfied(commonShaderProgram.locations.textureCoord != -1, "texture attribLocation not found");
		// check for error UNIFORMs
		// -> material
		printErrIfNotSatisfied(commonShaderProgram.locations.materialAmbient != -1, "material ambient uniformLocation not found");
		printErrIfNotSatisfied(commonShaderProgram.locations.materialDiffuse != -1, "material diffuse uniformLocation not found");
		printErrIfNotSatisfied(commonShaderProgram.locations.materialSpecular != -1, "material specular uniformLocation not found"); // RN removed by compiler => -1
		printErrIfNotSatisfied(commonShaderProgram.locations.materialShininess != -1, "material shininess uniformLocation not found"); // RN removed by compiler => -1
		// -> textures
		printErrIfNotSatisfied(commonShaderProgram.locations.textureEnabled != -1, "texture sampler uniformLocation not found"); // RN removed by compiler => -1
		// -> matrixes
		printErrIfNotSatisfied(commonShaderProgram.locations.PVM != -1, "PVM uniformLocation not found");
		printErrIfNotSatisfied(commonShaderProgram.locations.Vmatrix != -1, "Vmatrix uniformLocation not found");
		printErrIfNotSatisfied(commonShaderProgram.locations.Mmatrix != -1, "Mmatrix uniformLocation not found");
		printErrIfNotSatisfied(commonShaderProgram.locations.Nmatrix != -1, "Nmatrix uniformLocation not found");
		// -> spot light
		printErrIfNotSatisfied(commonShaderProgram.locations.lightAmbient != -1, "light ambient uniformLocation not found");
		printErrIfNotSatisfied(commonShaderProgram.locations.lightDiffuse != -1, "light diffuse uniformLocation not found");
		printErrIfNotSatisfied(commonShaderProgram.locations.lightSpecular != -1, "light specular uniformLocation not found");
		printErrIfNotSatisfied(commonShaderProgram.locations.lightPosition != -1, "light position uniformLocation not found");
		printErrIfNotSatisfied(commonShaderProgram.locations.lightDirection != -1, "light direction uniformLocation not found");

		commonShaderProgram.initialized = true;
	}
	
	{ // dynamic mesh shaders 
		GLuint shadersSphere[] = {
		  pgr::createShaderFromFile(GL_VERTEX_SHADER, "dynamicMesh.vert"),
		  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "sphere.frag"),
		  0
		};

		dynamicVertShaderProgram.program = pgr::createProgram(shadersSphere);

		// dynamic mesh specific
		dynamicVertShaderProgram.locations.elapsedTime = glGetUniformLocation(dynamicVertShaderProgram.program, "elapsedTime");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.elapsedTime != -1, "elapsedTime uniformLocation not found");

		// get location of the uniform (fragment) shader attributes
		dynamicVertShaderProgram.locations.textureSampler = glGetUniformLocation(dynamicVertShaderProgram.program, "tex");
		dynamicVertShaderProgram.locations.textureEnabled = glGetUniformLocation(dynamicVertShaderProgram.program, "texEnabled");

		dynamicVertShaderProgram.locations.position = glGetAttribLocation(dynamicVertShaderProgram.program, "aPos");
		dynamicVertShaderProgram.locations.normal = glGetAttribLocation(dynamicVertShaderProgram.program, "aNormal");
		dynamicVertShaderProgram.locations.textureCoord = glGetAttribLocation(dynamicVertShaderProgram.program, "aTexCoord");

		// other attributes and uniforms
		// -> material
		dynamicVertShaderProgram.locations.materialAmbient = glGetUniformLocation(dynamicVertShaderProgram.program, "material.ambient");
		dynamicVertShaderProgram.locations.materialDiffuse = glGetUniformLocation(dynamicVertShaderProgram.program, "material.diffuse");
		dynamicVertShaderProgram.locations.materialSpecular = glGetUniformLocation(dynamicVertShaderProgram.program, "material.specular");
		dynamicVertShaderProgram.locations.materialShininess = glGetUniformLocation(dynamicVertShaderProgram.program, "material.shininess");

		// -> matrixes
		dynamicVertShaderProgram.locations.PVM = glGetUniformLocation(dynamicVertShaderProgram.program, "PVM");
		dynamicVertShaderProgram.locations.Vmatrix = glGetUniformLocation(dynamicVertShaderProgram.program, "Vmatrix");
		dynamicVertShaderProgram.locations.Mmatrix = glGetUniformLocation(dynamicVertShaderProgram.program, "Mmatrix");
		dynamicVertShaderProgram.locations.Nmatrix = glGetUniformLocation(dynamicVertShaderProgram.program, "Nmatrix");
		// -> lights
		dynamicVertShaderProgram.locations.lightAmbient = glGetUniformLocation(dynamicVertShaderProgram.program, "spotLight.ambient");
		dynamicVertShaderProgram.locations.lightDiffuse = glGetUniformLocation(dynamicVertShaderProgram.program, "spotLight.diffuse");
		dynamicVertShaderProgram.locations.lightSpecular = glGetUniformLocation(dynamicVertShaderProgram.program, "spotLight.specular");
		dynamicVertShaderProgram.locations.lightPosition = glGetUniformLocation(dynamicVertShaderProgram.program, "spotLight.position");
		dynamicVertShaderProgram.locations.lightDirection = glGetUniformLocation(dynamicVertShaderProgram.program, "spotLight.direction");

		// check for error INs
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.position != -1, "position attribLocation not found");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.normal != -1, "normal attribLocation not found");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.textureCoord != -1, "texture attribLocation not found");
		// check for error UNIFORMs
		// -> material
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.materialAmbient != -1, "material ambient uniformLocation not found");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.materialDiffuse != -1, "material diffuse uniformLocation not found");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.materialSpecular != -1, "material specular uniformLocation not found"); // RN removed by compiler => -1
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.materialShininess != -1, "material shininess uniformLocation not found"); // RN removed by compiler => -1
		// -> textures
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.textureEnabled != -1, "texture sampler uniformLocation not found"); // RN removed by compiler => -1
		// -> matrixes
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.PVM != -1, "PVM uniformLocation not found");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.Vmatrix != -1, "Vmatrix uniformLocation not found");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.Mmatrix != -1, "Mmatrix uniformLocation not found");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.Nmatrix != -1, "Nmatrix uniformLocation not found");
		// -> spot light
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.lightAmbient != -1, "light ambient uniformLocation not found");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.lightDiffuse != -1, "light diffuse uniformLocation not found");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.lightSpecular != -1, "light specular uniformLocation not found");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.lightPosition != -1, "light position uniformLocation not found");
		printErrIfNotSatisfied(dynamicVertShaderProgram.locations.lightDirection != -1, "light direction uniformLocation not found");

		dynamicVertShaderProgram.initialized = true;
	}

	{ // dynamic texture shaders 
		GLuint shadersDynamicTexture[] = {
		  pgr::createShaderFromFile(GL_VERTEX_SHADER, "sphere.vert"),
		  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "dynamicTexture.frag"),
		  0
		};

		dynTexShaderProgram.program = pgr::createProgram(shadersDynamicTexture);
		
		// -> dynamic texture specific
		dynTexShaderProgram.locations.frame = glGetUniformLocation(dynTexShaderProgram.program, "frame");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.frame != -1, "frame uniformLocation not found");

		// get location of the uniform (fragment) shader attributes
		dynTexShaderProgram.locations.textureSampler = glGetUniformLocation(dynTexShaderProgram.program, "tex");
		dynTexShaderProgram.locations.textureEnabled = glGetUniformLocation(dynTexShaderProgram.program, "texEnabled");

		dynTexShaderProgram.locations.position = glGetAttribLocation(dynTexShaderProgram.program, "aPos");
		dynTexShaderProgram.locations.normal = glGetAttribLocation(dynTexShaderProgram.program, "aNormal");
		dynTexShaderProgram.locations.textureCoord = glGetAttribLocation(dynTexShaderProgram.program, "aTexCoord");

		// other attributes and uniforms
		// -> material
		dynTexShaderProgram.locations.materialAmbient = glGetUniformLocation(dynTexShaderProgram.program, "material.ambient");
		dynTexShaderProgram.locations.materialDiffuse = glGetUniformLocation(dynTexShaderProgram.program, "material.diffuse");
		dynTexShaderProgram.locations.materialSpecular = glGetUniformLocation(dynTexShaderProgram.program, "material.specular");
		dynTexShaderProgram.locations.materialShininess = glGetUniformLocation(dynTexShaderProgram.program, "material.shininess");

		// -> matrixes
		dynTexShaderProgram.locations.PVM = glGetUniformLocation(dynTexShaderProgram.program, "PVM");
		dynTexShaderProgram.locations.Vmatrix = glGetUniformLocation(dynTexShaderProgram.program, "Vmatrix");
		dynTexShaderProgram.locations.Mmatrix = glGetUniformLocation(dynTexShaderProgram.program, "Mmatrix");
		dynTexShaderProgram.locations.Nmatrix = glGetUniformLocation(dynTexShaderProgram.program, "Nmatrix");
		// -> lights
		dynTexShaderProgram.locations.lightAmbient = glGetUniformLocation(dynTexShaderProgram.program, "spotLight.ambient");
		dynTexShaderProgram.locations.lightDiffuse = glGetUniformLocation(dynTexShaderProgram.program, "spotLight.diffuse");
		dynTexShaderProgram.locations.lightSpecular = glGetUniformLocation(dynTexShaderProgram.program, "spotLight.specular");
		dynTexShaderProgram.locations.lightPosition = glGetUniformLocation(dynTexShaderProgram.program, "spotLight.position");
		dynTexShaderProgram.locations.lightDirection = glGetUniformLocation(dynTexShaderProgram.program, "spotLight.direction");

		// check for error INs
		printErrIfNotSatisfied(dynTexShaderProgram.locations.position != -1, "position attribLocation not found");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.normal != -1, "normal attribLocation not found");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.textureCoord != -1, "texture attribLocation not found");
		// check for error UNIFORMs
		// -> material
		printErrIfNotSatisfied(dynTexShaderProgram.locations.materialAmbient != -1, "material ambient uniformLocation not found");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.materialDiffuse != -1, "material diffuse uniformLocation not found");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.materialSpecular != -1, "material specular uniformLocation not found"); // RN removed by compiler => -1
		printErrIfNotSatisfied(dynTexShaderProgram.locations.materialShininess != -1, "material shininess uniformLocation not found"); // RN removed by compiler => -1
		// -> textures
		printErrIfNotSatisfied(dynTexShaderProgram.locations.textureEnabled != -1, "texture sampler uniformLocation not found"); // RN removed by compiler => -1
		// -> matrixes
		printErrIfNotSatisfied(dynTexShaderProgram.locations.PVM != -1, "PVM uniformLocation not found");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.Vmatrix != -1, "Vmatrix uniformLocation not found");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.Mmatrix != -1, "Mmatrix uniformLocation not found");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.Nmatrix != -1, "Nmatrix uniformLocation not found");
		// -> spot light
		printErrIfNotSatisfied(dynTexShaderProgram.locations.lightAmbient != -1, "light ambient uniformLocation not found");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.lightDiffuse != -1, "light diffuse uniformLocation not found");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.lightSpecular != -1, "light specular uniformLocation not found");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.lightPosition != -1, "light position uniformLocation not found");
		printErrIfNotSatisfied(dynTexShaderProgram.locations.lightDirection != -1, "light direction uniformLocation not found");

		dynTexShaderProgram.initialized = true;
	}
	
	{ // moving texture shaders 
		GLuint shaders[] = {
		  pgr::createShaderFromFile(GL_VERTEX_SHADER, "sphere.vert"),
		  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "movingTexture.frag"),
		  0
		};

		movTexShaderProgram.program = pgr::createProgram(shaders);

		// -> frame (mov tex specific)
		movTexShaderProgram.locations.scrollSpeed = glGetUniformLocation(movTexShaderProgram.program, "scrollSpeed");
		movTexShaderProgram.locations.elapsedTime = glGetUniformLocation(movTexShaderProgram.program, "elapsedTime");
		printErrIfNotSatisfied(movTexShaderProgram.locations.scrollSpeed != -1, "scrollSpeed uniformLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.elapsedTime != -1, "elapsedTime uniformLocation not found");

		// get location of the uniform (fragment) shader attributes
		movTexShaderProgram.locations.textureSampler = glGetUniformLocation(movTexShaderProgram.program, "tex");
		movTexShaderProgram.locations.textureEnabled = glGetUniformLocation(movTexShaderProgram.program, "texEnabled");

		movTexShaderProgram.locations.position = glGetAttribLocation(movTexShaderProgram.program, "aPos");
		movTexShaderProgram.locations.normal = glGetAttribLocation(movTexShaderProgram.program, "aNormal");
		movTexShaderProgram.locations.textureCoord = glGetAttribLocation(movTexShaderProgram.program, "aTexCoord");

		// other attributes and uniforms
		// -> material
		movTexShaderProgram.locations.materialAmbient = glGetUniformLocation(movTexShaderProgram.program, "material.ambient");
		movTexShaderProgram.locations.materialDiffuse = glGetUniformLocation(movTexShaderProgram.program, "material.diffuse");
		movTexShaderProgram.locations.materialSpecular = glGetUniformLocation(movTexShaderProgram.program, "material.specular");
		movTexShaderProgram.locations.materialShininess = glGetUniformLocation(movTexShaderProgram.program, "material.shininess");

		// -> matrixes
		movTexShaderProgram.locations.PVM = glGetUniformLocation(movTexShaderProgram.program, "PVM");
		movTexShaderProgram.locations.Vmatrix = glGetUniformLocation(movTexShaderProgram.program, "Vmatrix");
		movTexShaderProgram.locations.Mmatrix = glGetUniformLocation(movTexShaderProgram.program, "Mmatrix");
		movTexShaderProgram.locations.Nmatrix = glGetUniformLocation(movTexShaderProgram.program, "Nmatrix");
		// -> lights
		movTexShaderProgram.locations.lightAmbient = glGetUniformLocation(movTexShaderProgram.program, "spotLight.ambient");
		movTexShaderProgram.locations.lightDiffuse = glGetUniformLocation(movTexShaderProgram.program, "spotLight.diffuse");
		movTexShaderProgram.locations.lightSpecular = glGetUniformLocation(movTexShaderProgram.program, "spotLight.specular");
		movTexShaderProgram.locations.lightPosition = glGetUniformLocation(movTexShaderProgram.program, "spotLight.position");
		movTexShaderProgram.locations.lightDirection = glGetUniformLocation(movTexShaderProgram.program, "spotLight.direction");

		// check for error INs
		printErrIfNotSatisfied(movTexShaderProgram.locations.position != -1, "position attribLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.normal != -1, "normal attribLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.textureCoord != -1, "texture attribLocation not found");
		// check for error UNIFORMs
		// -> material
		printErrIfNotSatisfied(movTexShaderProgram.locations.materialAmbient != -1, "material ambient uniformLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.materialDiffuse != -1, "material diffuse uniformLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.materialSpecular != -1, "material specular uniformLocation not found"); // RN removed by compiler => -1
		printErrIfNotSatisfied(movTexShaderProgram.locations.materialShininess != -1, "material shininess uniformLocation not found"); // RN removed by compiler => -1
		// -> textures
		printErrIfNotSatisfied(movTexShaderProgram.locations.textureEnabled != -1, "texture sampler uniformLocation not found"); // RN removed by compiler => -1
		// -> matrixes
		printErrIfNotSatisfied(movTexShaderProgram.locations.PVM != -1, "PVM uniformLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.Vmatrix != -1, "Vmatrix uniformLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.Mmatrix != -1, "Mmatrix uniformLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.Nmatrix != -1, "Nmatrix uniformLocation not found");
		// -> spot light
		printErrIfNotSatisfied(movTexShaderProgram.locations.lightAmbient != -1, "light ambient uniformLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.lightDiffuse != -1, "light diffuse uniformLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.lightSpecular != -1, "light specular uniformLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.lightPosition != -1, "light position uniformLocation not found");
		printErrIfNotSatisfied(movTexShaderProgram.locations.lightDirection != -1, "light direction uniformLocation not found");

		movTexShaderProgram.initialized = true;
	}

	{ // INIT TEXTURES
		texturesInited.woodTexture = pgr::createTexture("textures/wood_floor_deck_diff_4k.jpg");
		texturesInited.brickTexture = pgr::createTexture("textures/pavement_04_diff_4k.jpg");
		texturesInited.wallRaw = pgr::createTexture("textures/wallRaw.jpg");
		texturesInited.skyboxTexture = pgr::createTexture("textures/skybox.jpg");
		texturesInited.dynamicTexture = pgr::createTexture("textures/dynamicTexture.png");
		texturesInited.movingTexture = pgr::createTexture("textures/movingTexture.png");
		texturesInited.rock = pgr::createTexture("textures/rock.jpg");
	}

	
	{ // skybox
		GLuint shadersSkybox[] = {
		  pgr::createShaderFromFile(GL_VERTEX_SHADER, "skybox.vert"),
		  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "skybox.frag"),
		  0
		};

		skyboxShaderProgram.program = pgr::createProgram(shadersSkybox);


		// get location of the uniform (fragment) shader attributes
		skyboxShaderProgram.locations.textureSampler = glGetUniformLocation(skyboxShaderProgram.program, "tex");
		skyboxShaderProgram.locations.textureEnabled = glGetUniformLocation(skyboxShaderProgram.program, "texEnabled");

		skyboxShaderProgram.locations.position = glGetAttribLocation(skyboxShaderProgram.program, "position");
		skyboxShaderProgram.locations.textureCoord = glGetAttribLocation(skyboxShaderProgram.program, "texCoord");

		// -> matrixes
		skyboxShaderProgram.locations.PVM = glGetUniformLocation(skyboxShaderProgram.program, "PVM");

		// check for error INs
		printErrIfNotSatisfied(skyboxShaderProgram.locations.position != -1, "[SKYBOX] position attribLocation not found");
		printErrIfNotSatisfied(skyboxShaderProgram.locations.textureCoord != -1, "[SKYBOX] texture attribLocation not found");
		// check for error UNIFORMs
		// -> textures
		printErrIfNotSatisfied(skyboxShaderProgram.locations.textureEnabled != -1, "[SKYBOX] texture enabled uniformLocation not found");
		printErrIfNotSatisfied(skyboxShaderProgram.locations.textureSampler != -1, "[SKYBOX] texture sampler uniformLocation not found");
		// -> matrixes
		printErrIfNotSatisfied(skyboxShaderProgram.locations.PVM != -1, "[SKYBOX] PVM uniformLocation not found");

		skyboxShaderProgram.initialized = true;
	}
}

/**
 * \brief Delete all shader program objects.
 */
void cleanupShaderPrograms(void) {
	pgr::deleteProgramAndShaders(skyboxShaderProgram.program);
	pgr::deleteProgramAndShaders(commonShaderProgram.program);
	pgr::deleteProgramAndShaders(dynTexShaderProgram.program);
	pgr::deleteProgramAndShaders(movTexShaderProgram.program);
	pgr::deleteProgramAndShaders(dynamicVertShaderProgram.program);
}

/**
 * \brief Draw all scene objects.
 */
void drawScene(void)
{
	glm::mat4 viewMatrix = cameras[gameState.activeCamera].getViewMatrixElevated();

	for (auto object : objects) {   // for (auto object : objects) {
		if (object != nullptr) {
			object->draw(viewMatrix, cameras[gameState.activeCamera].getProjectionMatrix());
		}
	}
}


// -----------------------  Window callbacks ---------------------------------

/**
 * \brief Draw the window contents.
 */
void displayCb() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw the window contents (scene objects)
	drawScene();

	glutSwapBuffers();
}

/**
 * \brief Window was reshaped.
 * \param newWidth New window width
 * \param newHeight New window height
 */
void reshapeCb(int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	for (Camera& camera : cameras) {
		camera.setProjectionMatrixRatio(float(newWidth), float(newHeight));
	}
	gameState.windowWidth = newWidth;
	gameState.windowHeight = newHeight;
};

// -----------------------  Mouse ---------------------------------
// three events - mouse click, mouse drag, and mouse move with no button pressed

/**
 * \brief React to mouse button press and release (mouse click).
 * When the user presses and releases mouse buttons in the window, each press
 * and each release generates a mouse callback (including release after dragging).
 *
 * \param buttonPressed button code (GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON)
 * \param buttonState GLUT_DOWN when pressed, GLUT_UP when released
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseCb(int buttonPressed, int buttonState, int mouseX, int mouseY) {
	if (buttonPressed == GLUT_LEFT_BUTTON) {
		if (buttonState == GLUT_DOWN) {
			doPicking(mouseX, mouseY);
		}
	}
	else if (buttonPressed == GLUT_RIGHT_BUTTON) {
		// TODO remove debug for object placing
		if (buttonState == GLUT_DOWN) {
			cout << glm::to_string(cameras[gameState.activeCamera].getPosition()) << endl;
		}
	}
}


/**
 * \brief Handle mouse movement over the window (with no button pressed).
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void passiveMouseMotionCb(int mouseX, int mouseY) {

	switch (gameState.activeCamera)
	{
		case CAMERA_FREE_IDX: {
			glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
			glutSetCursor(GLUT_CURSOR_NONE);

			cameras[CAMERA_FREE_IDX].increaseElevation((float)(WINDOW_HEIGHT / 2 - (float)mouseY) / 10);
			cameras[CAMERA_FREE_IDX].rotateHorizontal(((float)mouseX - WINDOW_WIDTH / 2) / 1000);
			break;
		}
		default: {
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
			break;
		}
	}
}


// -----------------------  Keyboard ---------------------------------

/**
 * \brief Handle the key pressed event.
 * Called whenever a key on the keyboard was pressed. The key is given by the "keyPressed"
 * parameter, which is an ASCII character. It's often a good idea to have the escape key (ASCII value 27)
 * to call glutLeaveMainLoop() to exit the program.
 * \param keyPressed ASCII code of the key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardCb(unsigned char keyPressed, int mouseX, int mouseY) {

	if (keyPressed == 27) { // escape
		glutLeaveMainLoop();
		exit(EXIT_SUCCESS);
	}
	cout << "Pressed key: " << keyPressed << endl;
	switch (keyPressed) {
		//> movement START
	case 'd':
		gameState.keyMap[KEY_RIGHT_ARROW] = true;
		break;
	case 'a':
		gameState.keyMap[KEY_LEFT_ARROW] = true;
		break;
	case 'w':
		gameState.keyMap[KEY_UP_ARROW] = true;
		break;
	case 's':
		gameState.keyMap[KEY_DOWN_ARROW] = true;
		break;
		//< movement END

	case 'c': // collision toggle
		gameState.collisionEnabled = !gameState.collisionEnabled;
		break;
	default:
		break;
	}
}

// Called whenever a key on the keyboard was released. The key is given by
// the "keyReleased" parameter, which is in ASCII. 
/**
 * \brief Handle the key released event.
 * \param keyReleased ASCII code of the released key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardUpCb(unsigned char keyReleased, int mouseX, int mouseY) {
	switch (keyReleased) {
	case 'd':
		gameState.keyMap[KEY_RIGHT_ARROW] = false;
		break;
	case 'a':
		gameState.keyMap[KEY_LEFT_ARROW] = false;
		break;
	case 'w':
		gameState.keyMap[KEY_UP_ARROW] = false;
		break;
	case 's':
		gameState.keyMap[KEY_DOWN_ARROW] = false;
		break;
	default:
		break;
	}
}

/**
 * \brief Handle the non-ASCII key pressed event (such as arrows or F1).
 *  The special keyboard callback is triggered when keyboard function (Fx) or directional
 *  keys are pressed.
 * \param specKeyPressed int value of a predefined glut constant such as GLUT_KEY_UP
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void specialKeyboardCb(int specKeyPressed, int mouseX, int mouseY) {
	switch (specKeyPressed) {
		case GLUT_KEY_RIGHT: {
			gameState.keyMap[KEY_RIGHT_ARROW] = true;
			break;
		}
		case GLUT_KEY_LEFT: {
			gameState.keyMap[KEY_LEFT_ARROW] = true;
			break;
		}
		case GLUT_KEY_UP: {
			gameState.keyMap[KEY_UP_ARROW] = true;
			break;
		}
		case GLUT_KEY_DOWN: {
			gameState.keyMap[KEY_DOWN_ARROW] = true;
			break;
		}
		case GLUT_KEY_F1: {
			int oldActiveCameraIdx = gameState.activeCamera;
			gameState.activeCamera = CAMERA_FREE_IDX;
			cameras[gameState.activeCamera].setCameraFrom(cameras[oldActiveCameraIdx]);
			cameras[gameState.activeCamera].ground();
			break;
		}
		case GLUT_KEY_F2: {
			gameState.activeCamera = CAMERA_2_IDX;
			break;
		}
		case GLUT_KEY_F3: {
			gameState.activeCamera = CAMERA_3_IDX;
			break;
		}
		case GLUT_KEY_F4: {
			gameState.activeCamera = CAMERA_4_MOVING_IDX;
			break;
		}
		default:
			break;
	}
}

/**
 * \brief Handle the non-ASCII key pressed event (such as arrows or F1).
 *  The special keyboard callback is triggered when keyboard function (Fx) or directional
 *  keys are released.
 * \param specKeyPressed int value of a predefined glut constant such as GLUT_KEY_UP
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void specialKeyboardUpCb(int specKeyReleased, int mouseX, int mouseY) {
	switch (specKeyReleased) {
	case GLUT_KEY_RIGHT:
		gameState.keyMap[KEY_RIGHT_ARROW] = false;
		break;
	case GLUT_KEY_LEFT:
		gameState.keyMap[KEY_LEFT_ARROW] = false;
		break;
	case GLUT_KEY_UP:
		gameState.keyMap[KEY_UP_ARROW] = false;
		break;
	case GLUT_KEY_DOWN:
		gameState.keyMap[KEY_DOWN_ARROW] = false;
		break;
	default:
		break;
	}
}

/**
 * \brief Updates given camera position and other properties.
 * \param index of camera
 * \param delta time
 */
void updateCamera(int cameraIdx, float deltaTime) {
	switch (cameraIdx)
	{
		case CAMERA_FREE_IDX:
		{
			glm::vec3 moveDir = glm::vec3(0.0f);

			if (gameState.keyMap[KEY_LEFT_ARROW] == true) {
				moveDir += cameras[CAMERA_FREE_IDX].getLeft();
			}

			if (gameState.keyMap[KEY_RIGHT_ARROW] == true) {
				moveDir -= cameras[CAMERA_FREE_IDX].getLeft();
			}

			if (gameState.keyMap[KEY_UP_ARROW] == true) {
				moveDir -= cameras[CAMERA_FREE_IDX].getFront();
			}

			if (gameState.keyMap[KEY_DOWN_ARROW] == true) {
				moveDir += cameras[CAMERA_FREE_IDX].getFront();
			}

			if (moveDir.length() != 0.0f) {
				glm::normalize(moveDir);
				cameras[CAMERA_FREE_IDX].move(moveDir * CAMERA_FREE_SPEED * deltaTime);
				if (gameState.collisionEnabled) {
					auto newPosition = cameras[CAMERA_FREE_IDX].getPosition();
					newPosition.x = glm::clamp(newPosition.x, -6.0f, 2.5f);
					newPosition.z = glm::clamp(newPosition.z, -3.2f, 4.4f);
					cameras[CAMERA_FREE_IDX].setPosition(newPosition);
				}
			}
			break;
		}
		case CAMERA_4_MOVING_IDX: {
			cameras[CAMERA_4_MOVING_IDX].setPositionAsReferObj();
			break;
		}
		case CAMERA_2_IDX:
		case CAMERA_3_IDX:
		default:
			//cerr << "update of camera num: " << cameraIdx << " not yet implemented" << endl;
			break;
	}
}

// -----------------------  Timer ---------------------------------

/**
 * \brief Callback responsible for the scene update.
 */
void timerCb(int)
{

	const glm::mat4 sceneRootMatrix = glm::mat4(1.0f);
	float elapsedTime = 0.001f * static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // milliseconds => seconds

	// update camera
	float cameraDeltaTime = elapsedTime - cameras[0].currentTime;
	updateCamera(gameState.activeCamera, cameraDeltaTime);

	for (int i = 0; i < CAMERA_COUNT; ++i) {
		cameras[i].currentTime = elapsedTime;
	}

	gameState.skybox->setPosition(
		cameras[gameState.activeCamera].getPosition()
	);

	// update the application state
	for (auto object : objects) {
		if (object != nullptr)
			object->update(elapsedTime, &sceneRootMatrix);
	}

	// and plan a new event
	glutTimerFunc(33, timerCb, 0);

	// create display event
	glutPostRedisplay();
}

// -----------------------  Application ---------------------------------

/**
 * \brief Place objects to the scene
 */
void initObjects() {
	// CAMERAS
	cameras[CAMERA_FREE_IDX] = Camera(CAMERA_FREE_INIT_POSITION);
	cameras[CAMERA_2_IDX] = Camera(CAMERA_2_INIT_POSITION, CAMERA_2_INIT_DIRECTION);
	cameras[CAMERA_3_IDX] = Camera(CAMERA_3_INIT_POSITION, CAMERA_3_INIT_DIRECTION);
	cameras[CAMERA_4_MOVING_IDX] = Camera(CAMERA_3_INIT_POSITION, CAMERA_3_INIT_DIRECTION);

	for (Camera& camera : cameras) {
		camera.setProjectionMatrixRatio(WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	// OTHERS
	{ // selectable and movable pair
		// moving object
		auto movObj = make_shared<MovingObject>(&commonShaderProgram, "models/cucumber.fbx");
		
		//movObj->rotateYAxis(90.0f);
		movObj->scale(3.0f);
		movObj->setTexture(texturesInited.wallRaw);
		
		cameras[CAMERA_4_MOVING_IDX].setRefObject(movObj);
		objects.push_back(movObj);
	}

	{
		// dynamic texture "TV"
		const int numFrames = 4;
		auto dynCube = make_shared<MeshDynTex>(&dynTexShaderProgram, "models/cubeDynamicTexture.fbx", numFrames);
		dynCube->scale(0.4f, 2.0f, 2.0f);
		dynCube->setPosition(3.5f, 2.0f, 0.0f);
		dynCube->setTexture(texturesInited.dynamicTexture);

		dynCube->setMaterial(
			glm::vec3(0.1f, 0.1f, 0.1f),
			glm::vec3(0.8f, 0.8f, 0.8f),
			glm::vec3(0.2f, 0.2f, 0.2f),
			10.0f
		);
		objects.push_back(dynCube);

		// button
		auto button = make_shared<SelectableObject>(&commonShaderProgram, "models/button.fbx");
		button->rotateZAxis(90.0f);
		button->scale(0.2f);
		button->setXPosition(4.0f);
		button->setZPosition(-3.0f);

		std::function<void()> boundFunction = std::bind(&MeshDynTex::toggleEnabled, dynCube);
		button->setFunction(boundFunction);

		ObjectMaterial* matteMetalMaterial = new ObjectMaterial;
		matteMetalMaterial->ambient = glm::vec3(0.3f, 0.3f, 0.3f);
		matteMetalMaterial->diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
		matteMetalMaterial->specular = glm::vec3(0.1f, 0.1f, 0.1f);
		matteMetalMaterial->shininess = 20.0f;

		button->setMaterial(matteMetalMaterial);

		objects.push_back(button);
	}

	{ // wood sphere
		auto sphere = make_shared<Sphere>(&commonShaderProgram);

		sphere->setMaterial(
			glm::vec3(0.0f, 0.1f, 0.3f),
			glm::vec3(0.0f, 0.6f, 0.9f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			32.0f
		);

		sphere->setPosition(3.0f, 0.0f, 3.0f);
		sphere->setTexture(texturesInited.woodTexture);
		objects.push_back(sphere);
	}

	{ // skybox 
		gameState.skybox = make_shared<Skybox>(&skyboxShaderProgram, "models/skyBox.fbx");
		gameState.skybox->setTexture(texturesInited.skyboxTexture);
		gameState.skybox->scale(60.0f);
		objects.push_back(gameState.skybox);
	}

	auto movTex = initMovTexObj(&movTexShaderProgram, texturesInited.movingTexture);
	objects.push_back(movTex);

	{ // platform & tetrahedron [SELECTABLE]
		auto tetrahedron = make_shared<Tetrahedron>(&commonShaderProgram);

		tetrahedron->setMaterial(
			glm::vec3(0.0f, 0.1f, 0.3f),
			glm::vec3(0.0f, 0.6f, 0.9f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			32.0f
		);
		tetrahedron->setPosition(-3.0f, 0.5f, -2.5f);
		objects.push_back(tetrahedron);

		std::function<void()> boundFunction = [&]() {
			for (auto& obj : objects) {
				if (dynamic_pointer_cast<Tetrahedron>(obj)) {
					obj = make_shared<Tetrahedron>(&commonShaderProgram);
					obj->setMaterial(
						glm::vec3(0.0f, 0.1f, 0.3f),
						glm::vec3(0.0f, 0.6f, 0.9f),
						glm::vec3(0.5f, 0.5f, 0.5f),
						32.0f
					);
					obj->setPosition(-3.0f, 0.5f, -2.5f);
					//obj->setTexture(texturesInited.skyboxTexture);
				}
			}
		};

		auto platform = make_shared<SelectableObject>(&commonShaderProgram, "models/platformRound.fbx");

		platform->setFunction(boundFunction);
		platform->setPosition(-3.0f, -1.0f, -2.5f);
		platform->setTexture(texturesInited.wallRaw);
		objects.push_back(platform);
	}

	auto plane = initPlane(&dynamicVertShaderProgram);
	objects.push_back(plane);

	auto sofa = initSofa(&commonShaderProgram);
	objects.push_back(sofa);

	auto railing = initRailing(&commonShaderProgram, texturesInited.woodTexture);
	objects.push_back(railing);

	auto tower = initTower(&commonShaderProgram, texturesInited.wallRaw);
	objects.push_back(tower);
	

	{
		// ASTEROIDS
		auto randDisplacementXZ = std::make_unique<RealGenerator>(ASTEROIDS_MIN_DISPLACEMENT_HORIZONTAL, ASTEROIDS_MAX_DISPLACEMENT_HORIZONTAL);
		auto randDisplacementY = std::make_unique<RealGenerator>(ASTEROIDS_MIN_DISPLACEMENT_VERTICAL, ASTEROIDS_MAX_DISPLACEMENT_VERTICAL);

		auto randScale = std::make_unique<RealGenerator>(ASTEROIDS_SCALE_MIN, ASTEROIDS_SCALE_MAX);
		auto randRotate = std::make_unique<RealGenerator>(ASTEROIDS_ANGLE_MIN, ASTEROIDS_ANGLE_MAX);

		for (int z = ASTEROIDS_Z_MIN; z <= ASTEROIDS_Z_MAX; z += ASTEROIDS_Z_STEP) {
			for (int x = ASTEROIDS_X_MIN; x <= ASTEROIDS_X_MAX; x += ASTEROIDS_X_STEP) {
				auto asteroid = make_shared<SingleMesh>(&commonShaderProgram, "models/Monster.fbx");

				// rand rotate
				asteroid->rotateXAxis(randRotate->getNext());
				asteroid->rotateYAxis(randRotate->getNext());
				asteroid->rotateZAxis(randRotate->getNext());

				// rand scale
				asteroid->scale(randScale->getNext());

				// rand position
				asteroid->setPosition(
					float(x) + randDisplacementXZ->getNext(),
					0.0f + randDisplacementY->getNext(),
					float(z) + randDisplacementXZ->getNext()
				);
				asteroid->setTexture(texturesInited.rock);

				objects.push_back(asteroid);
			}
		}
	}

	{
		 // random selectable object
		 auto selectableObject = make_shared<SelectableObject>(&commonShaderProgram, "models/monster.fbx");
		 std::function<void()> boundFunction = std::bind(&SelectableObject::moveUp, selectableObject);
		 selectableObject->setTexture(texturesInited.rock);
		 selectableObject->setFunction(boundFunction);
		 selectableObject->setYPosition(2.0f);

		objects.push_back(selectableObject);
	}
}

/**
 * \brief Initialize application data and OpenGL stuff.
 */
void initApplication() {
	// init OpenGL
	loadShaderPrograms();
	glEnable(GL_DEPTH_TEST);
	initObjects();
}

/**
 * \brief Delete all OpenGL objects and application data.
 */
void finalizeApplication(void) {
	objects.clear();
	// delete shaders
	cleanupShaderPrograms();
}


/**
 * \brief Entry point of the application.
 * \param argc number of command line arguments
 * \param argv array with argument strings
 * \return 0 if OK, <> elsewhere
 */
int main(int argc, char** argv) {

	// initialize the GLUT library (windowing system)
	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	// for each window
	{
		//   initial window size + title
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		glutCreateWindow(WINDOW_TITLE);

		// callbacks - use only those you need
		glutDisplayFunc(displayCb);
		glutReshapeFunc(reshapeCb);
		glutKeyboardFunc(keyboardCb);
		glutKeyboardUpFunc(keyboardUpCb);
		glutSpecialFunc(specialKeyboardCb);     // key pressed
		glutSpecialUpFunc(specialKeyboardUpCb); // key released
		 glutMouseFunc(mouseCb);
		glutPassiveMotionFunc(passiveMouseMotionCb);

		glutTimerFunc(33, timerCb, 0);
	}
	// end for each window 

	// initialize pgr-framework (GL, DevIl, etc.)
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");

	// init your stuff - shaders & program, buffers, locations, state of the application
	initApplication();

	// handle window close by the user
	glutCloseFunc(finalizeApplication);

	// Infinite loop handling the events
	glutMainLoop();

	return EXIT_SUCCESS;
}
