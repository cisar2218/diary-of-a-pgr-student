//----------------------------------------------------------------------------------------
/**
 * \file    toyshop.cpp : This file contains the 'main' function and callbacks.
			Program execution begins and ends there.
 * \author  Dušan Jánsky
 * \date    2022/04/02
 * \brief   Semestral work for PGR class.
 */

 /**
  * \brief	\mainpage Documentation of the skeleton for the Computer graphics course on FEE and FIT CTU CZ
  *
  * This is the skeleton of an OpenGL application written in FreeGLUT and PGR framework.
  * It serves as a starter boiler-plate code for a semester project - a simple interactive application.
  * Write your code into the functions prepared. Add your data (models, textures, ...) and logic.
  *
  *
  * Comment your code using the [doxygen](https://www.doxygen.nl/index.html) documenting system style.
  * Create "doxygen" directory, make it current by "cd doxygen", prepare a configuration file with "doxygen -g" and edit the details.
  *
  * Start by renaming of this file from skeleton.cpp to <your_name>.cpp and the project to <your_name>.vcxproj
  *
  * In <your_name>.vcxproj:
  *   - rename skeleton in <RootNamespace>skeleton</RootNamespace> to <your_name>
  *   - replace skeleton.cpp in <ClCompile Include="skeleton.cpp" /> to <your_name>.cpp
  *
  * Start coding and documenting. Generate the documentation by the command "doxygen" in the "doxygen" directory.
  *
  */


  // TODO: tabulka klaves a jeji obsluha keyPressed/keyReleased a timer


#include <iostream>
#include "pgr.h"
#include "object.h"
#include "triangle.h"
#include "square.h"
#include "singlemesh.h"
#include "camera.h"
#include "parameters.h"
#include "janskdus.h"
using namespace std;

enum { KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, KEY_DOWN_ARROW, KEYS_COUNT };
enum { CAMERA_FREE_IDX, CAMERA_2_IDX, CAMERA_3_IDX, CAMERA_COUNT };

constexpr int WINDOW_WIDTH = 500;
constexpr int WINDOW_HEIGHT = 500;
constexpr char WINDOW_TITLE[] = "PGR: Application Skeleton";

ObjectList objects;

// shared shader programs
ShaderProgram commonShaderProgram;

Camera cameras[CAMERA_COUNT];

struct GameState {
	int windowWidth;    // set by reshape callback
	int windowHeight;   // set by reshape callback

	int activeCamera = CAMERA_FREE_IDX;
	float cameraElevationAngle; // in degrees = initially 0.0f

	bool keyMap[KEYS_COUNT];    // false
} gameState;

// -----------------------  OpenGL stuff ---------------------------------

/**
 * \brief Load and compile shader programs. Get attribute locations.
 */
void loadShaderPrograms()
{
	GLuint shaders[] = {
	  pgr::createShaderFromFile(GL_VERTEX_SHADER, "simple.vs"),
	  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "simple.fs"),
	  0
	};

	commonShaderProgram.program = pgr::createProgram(shaders);
	commonShaderProgram.locations.position = glGetAttribLocation(commonShaderProgram.program, "position");

	// other attributes and uniforms
	commonShaderProgram.locations.PVMmatrix = glGetUniformLocation(commonShaderProgram.program, "PVM");

	assert(commonShaderProgram.locations.PVMmatrix != -1);
	assert(commonShaderProgram.locations.position != -1);
	// ...

	commonShaderProgram.initialized = true;
}

/**
 * \brief Delete all shader program objects.
 */
void cleanupShaderPrograms(void) {

	pgr::deleteProgramAndShaders(commonShaderProgram.program);
}

/**
 * \brief Draw all scene objects.
 */
void drawScene(void)
{
	glm::mat4 viewMatrix = cameras[gameState.activeCamera].getViewMatrixElevated();

	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->draw(viewMatrix, cameras[gameState.activeCamera].getProjectionMatrix());
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
	// TODO: Take new window size and update the application state,
	// window and projection.

	// glViewport(...);
};

// -----------------------  Mouse ---------------------------------
// three events - mouse click, mouse drag, and mouse move with no button pressed

// 
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
}

/**
 * \brief Handle mouse dragging (mouse move with any button pressed).
 *        This event follows the glutMouseFunc(mouseCb) event.
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseMotionCb(int mouseX, int mouseY) {
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

	// create display event to redraw window contents if needed (and not handled in the timer callback)
	// glutPostRedisplay();
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

//
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
		case GLUT_KEY_RIGHT:
			gameState.keyMap[KEY_RIGHT_ARROW] = true;
			break;
		case GLUT_KEY_LEFT:
			gameState.keyMap[KEY_LEFT_ARROW] = true;
			break;
		case GLUT_KEY_UP:
			gameState.keyMap[KEY_UP_ARROW] = true;
			break;
		case GLUT_KEY_DOWN:
			gameState.keyMap[KEY_DOWN_ARROW] = true;
			break;
		case GLUT_KEY_F1:
			gameState.activeCamera = CAMERA_FREE_IDX;
			break;
		case GLUT_KEY_F2:
			gameState.activeCamera = CAMERA_2_IDX;
			break;
		case GLUT_KEY_F3:
			gameState.activeCamera = CAMERA_3_IDX;
			break;
		default:
			break;
	}
}

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
				cout << "here" << endl;
			}
			if (moveDir.length() != 0.0f) {
				glm::normalize(moveDir);
				cameras[CAMERA_FREE_IDX].move(moveDir * CAMERA_FREE_SPEED * deltaTime);
			}
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
#ifndef SKELETON // @task_1_0
	const glm::mat4 sceneRootMatrix = glm::mat4(1.0f);
	float elapsedTime = 0.001f * static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // milliseconds => seconds

	// update camera
	float cameraDeltaTime = elapsedTime - cameras[0].currentTime;
	updateCamera(gameState.activeCamera, cameraDeltaTime);

	for (int i = 0; i < CAMERA_COUNT; ++i) {
		cameras[i].currentTime = elapsedTime;
	}

	

	// update the application state
	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->update(elapsedTime, &sceneRootMatrix);
	}
#endif // task_1_0

	// and plan a new event
	glutTimerFunc(33, timerCb, 0);

	// create display event
	glutPostRedisplay();
}


// -----------------------  Application ---------------------------------

/**
 * \brief Initialize application data and OpenGL stuff.
 */
void initApplication() {
	// init OpenGL
	// - all programs (shaders), buffers, textures, ...
	loadShaderPrograms();

	//objects.push_back(new Triangle(&commonShaderProgram));
	//objects.push_back(new Square(&commonShaderProgram));
	objects.push_back(new SingleMesh(&commonShaderProgram));
	// objects.push_back(new SingleMesh(&commonShaderProgram));

	// init your Application
	// - setup the initial application state
	
	// CAMERAS
	cameras[CAMERA_FREE_IDX] = Camera(CAMERA_FREE_INIT_POSITION);
	cameras[CAMERA_2_IDX] = Camera(CAMERA_2_INIT_POSITION, CAMERA_2_INIT_DIRECTION);
	cameras[CAMERA_3_IDX] = Camera(CAMERA_3_INIT_POSITION, CAMERA_3_INIT_DIRECTION);
}

/**
 * \brief Delete all OpenGL objects and application data.
 */
void finalizeApplication(void) {

	// cleanUpObjects();

	// delete buffers
	// cleanupModels();

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

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

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
		// glutMouseFunc(mouseCb);
		// glutMotionFunc(mouseMotionCb);
		glutPassiveMotionFunc(passiveMouseMotionCb);

#ifndef SKELETON // @task_1_0
		glutTimerFunc(33, timerCb, 0);
#else
		// glutTimerFunc(33, timerCb, 0);
#endif // task_1_0

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

	// code after glutLeaveMainLoop()
	// cleanup

	return EXIT_SUCCESS;
}
