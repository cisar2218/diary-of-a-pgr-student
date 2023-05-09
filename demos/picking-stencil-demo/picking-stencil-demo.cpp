//----------------------------------------------------------------------------------------
/**
 * \file    picking-stencil-demo.cpp
 * \author  Tomas Barak, Petr Felkel, Jaroslav Sloup
 * \date    2012-2013
 * \brief   Per pixel correct picking, done by means of the stencil buffer.
 */
//----------------------------------------------------------------------------------------

// restrictions/limitations of this technique:
// - max number of pickable objects in the scene is 255 (ID 1-255, 0-background),
//   because of 8 bits per pixel in the stencil buffer
// - we will not be able to get a precise hit location in world space
//   (we only know whether an object with a particular ID was clicked on)

#include <iostream>

#include "pgr.h"

// projection matrix used by shaders
glm::mat4 projection;
int winWidth, winHeight;

#define SCENE_UPDATE_TIME 33 // milliseconds

// our scene is made of several boxes, which will start to rotate on clik
struct Box {
  // costructor with Model matrix as parameter
  Box(const glm::mat4 & model_mat): model(model_mat), rotation(glm::vec3(0, 0, 0)) {}

  // each box has its own transformation matrix
  glm::mat4 model;
  // each box has its rotation vector
  glm::vec3 rotation;

  // all boxes share these variables
  static GLuint draw_shader;        // this shader is used to draw normal scene image (shaded boxes)
  static GLuint buffer_id;          // vertex buffer object (storing vertex coords and normals as a block of bytes)

  // we use 2 shaders, locations can differ
  static GLint model_mat_loc_draw;  // location of model matrix in draw shader
  static GLint proj_mat_loc_draw;   // location of projection matrix in draw shader
  static GLint pos_loc_draw;        // location of position attribute in draw shader
  static GLint nor_loc_draw;        // location of normal attribute in draw shader
  static GLuint vertexArray_draw;   // Vertex Array Object (buffer binding to attributes + locations)
};

// Box class variables (static)
GLuint Box::draw_shader   = 0;
GLuint Box::buffer_id     = 0;

GLint Box::model_mat_loc_draw = 0;
GLint Box::proj_mat_loc_draw  = 0;
GLint Box::pos_loc_draw   = 0;
GLint Box::nor_loc_draw   = 0;

GLuint Box::vertexArray_draw = 0; // Vertex Array Object (buffer binding to attributes + locations)

// few randomly spawn boxes
Box boxes[] = {
  Box(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10))),
  Box(glm::translate(glm::mat4(1.0f), glm::vec3(-2.5, 1.5, -8))),
  Box(glm::translate(glm::mat4(1.0f), glm::vec3(-2.5, -1, -8))),
  Box(glm::translate(glm::mat4(1.0f), glm::vec3(2.5, -1.5, -8))),
  Box(glm::translate(glm::mat4(1.0f), glm::vec3(2.5, -1, -10)))
};

extern float box_data[]; // raw box vertices and normals are listed in separate file (box_data.cpp)

// vertex shader used for drawing - calculates position and normal
static const std::string drawVtxShader(
    "#version 140\n"
    "uniform mat4 M;\n"
    "uniform mat4 P;\n"
    "in vec3 position;\n"
    "in vec3 normal;\n"
    "smooth out vec3 theNormal;\n"
    "smooth out vec3 thePosition;\n"
    "void main()\n"
    "{\n"
    "  vec4 pos4 = M * vec4(position, 1);\n"
    "  gl_Position = P * pos4;\n"
    //"  thePosition = pos4.xyz / pos4.w;\n"  // It is hard to get w <> 1 after multiplication by M - e.g. shear or asymetric scale
    "  thePosition = pos4.xyz;\n"
    "  theNormal = (M * vec4(normal, 0)).xyz;\n"
    "}\n"
);

// fragment shader uses normal, position, and hardcoded light to calculate simple shading
static const std::string drawFragmentShader(
    "#version 140\n"
    "smooth in vec3 theNormal;\n"
    "smooth in vec3 thePosition;\n"
    "out vec4 outputColor;\n"
    "const vec3 lightPos = vec3(0, 2, 0);\n"
    "void main()\n"
    "{\n"
    "  outputColor = vec4(1, 1, 1, 1) * max(0.0, dot(normalize(theNormal), normalize(lightPos - thePosition)));\n"
    "}\n"
);

void init() {
  // build draw shader and query all attribute and uniform locations
  std::vector<GLuint> shaderList;
  shaderList.push_back(pgr::createShaderFromSource(GL_VERTEX_SHADER, drawVtxShader));
  shaderList.push_back(pgr::createShaderFromSource(GL_FRAGMENT_SHADER, drawFragmentShader));

  Box::draw_shader = pgr::createProgram(shaderList);
  Box::model_mat_loc_draw = glGetUniformLocation(Box::draw_shader, "M");
  Box::proj_mat_loc_draw = glGetUniformLocation(Box::draw_shader, "P");
  Box::pos_loc_draw = glGetAttribLocation(Box::draw_shader, "position");
  Box::nor_loc_draw = glGetAttribLocation(Box::draw_shader, "normal");

  // create VAO for drawing the boxes
  glGenVertexArrays(1, &Box::vertexArray_draw);
  glBindVertexArray(Box::vertexArray_draw);
  // build vertex buffer and upload data to it
  glGenBuffers(1, &Box::buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, Box::buffer_id);
  glBufferData(GL_ARRAY_BUFFER, 6*6*6 * sizeof(float), box_data, GL_STATIC_DRAW);
  // enable position and normal arrays
  glEnableVertexAttribArray(Box::pos_loc_draw);
  glEnableVertexAttribArray(Box::nor_loc_draw);

  // describe the attribute position and normal in the vertex buffer and its layout
  glVertexAttribPointer(Box::pos_loc_draw, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(Box::nor_loc_draw, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * 6 * 6 * sizeof(float)));
  glBindVertexArray(0);

  glDisable(GL_CULL_FACE); // draw both back and front faces
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
}

void doPicking(int button, int x, int y) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearStencil(0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glUseProgram(Box::draw_shader);

  glUniformMatrix4fv(Box::proj_mat_loc_draw, 1, GL_FALSE, glm::value_ptr(projection));

  // enable stencil test
  glEnable(GL_STENCIL_TEST);
  // if the stencil test and depth test are passed than value in the stencil
  // buffer is replaced with the object ID (byte 1..255)
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  glBindVertexArray(Box::vertexArray_draw);

  for(int b = 0; b < sizeof(boxes) / sizeof(Box); ++b) {
    // stencil test always passes
    // reference value for stencil test is set to be object ID (b+1)
    // -> this value is used to update stencil buffer contents
    glStencilFunc(GL_ALWAYS, b + 1, -1);

    glUniformMatrix4fv(Box::model_mat_loc_draw, 1, GL_FALSE, glm::value_ptr(boxes[b].model));
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6); // six vertices per box side, six sides of the box
  }

  glBindVertexArray(0);

  glFinish();

  // read value from the stencil buffer for one pixel under the mouse cursor
  unsigned char pixelID;
  glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &pixelID);

  // disable stencil test
  glDisable(GL_STENCIL_TEST);

  // the buffer was cleared to zeros
  if(pixelID == 0) { // background was clicked
    std::cout << "clicked on background" << std::endl;
  } else { // object was clicked
    if(button == GLUT_LEFT_BUTTON)
      boxes[pixelID-1].rotation = glm::vec3(1, 1, 1); // start box rotation on left mouse click
    else
      boxes[pixelID-1].rotation = glm::vec3(0, 0, 0); // and stop it on the right click

    // viewport transformation
    glm::vec4 viewport = glm::vec4(0, 0, winWidth, winHeight);

    GLfloat winZ;  // pixel depth coordinate
    // read depth coordinate from depth buffer
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    // window coordinates of the mouse click
    glm::vec3 winCoords = glm::vec3(x, y, winZ);

    // map the specified window coordinates into world/object coordinates
    glm::vec3 objCoords = glm::unProject(winCoords, boxes[pixelID-1].model, projection, viewport);

    std::cout << "clicked on object with ID: " << (int)pixelID << std::endl;

    std::cout << "# screen coordinates [" << winCoords.x << ", " << winCoords.y << ", " << winCoords.z << "]" << std::endl;
    std::cout << " -> object coordinates [" << objCoords.x << ", " << objCoords.y << ", " << objCoords.z << "]" << std::endl;
  }
}

void timerCallback(int) {
  // animate boxes
  for(int b = 0; b < sizeof(boxes) / sizeof(Box); ++b) {
    if(boxes[b].rotation.x == 0 && boxes[b].rotation.y == 0 && boxes[b].rotation.z == 0)
      continue;

    boxes[b].model = glm::rotate(boxes[b].model, 1.0f, boxes[b].rotation);
  }

  glutTimerFunc(SCENE_UPDATE_TIME, timerCallback, 0);
  glutPostRedisplay();
}

void keyboardCallback(unsigned char key, int x, int y) {
  // exit on escape
  if(key == 27)
    exit(0);
}

void mouseCallback(int button, int state, int x, int y) {
  // do picking only on mouse down
  if(state == GLUT_DOWN) {
    doPicking(button, x, winHeight - 1 - y); // recalculate y, as glut has origin in upper left corner
    glutPostRedisplay();
  }
}

void displayCallback() {
  // we render our scene as usual
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(Box::draw_shader);

  glUniformMatrix4fv(Box::proj_mat_loc_draw, 1, GL_FALSE, glm::value_ptr(projection));

  glBindVertexArray( Box::vertexArray_draw );
  for(int b = 0; b < sizeof(boxes) / sizeof(Box); ++b) {
    glUniformMatrix4fv(Box::model_mat_loc_draw, 1, GL_FALSE, glm::value_ptr(boxes[b].model));
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6); // six vertices per box side, six sides of the box
  }
  glBindVertexArray(0);

  glutSwapBuffers();
}

void reshapeCallback(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  float aspect_ratio = (float)w/(float)h;
  projection = glm::perspective(45.0f, aspect_ratio, 1.0f, 20.0f);

  winWidth = w;
  winHeight = h;
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);

  glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Picking demo - stencil buffer");

  glutDisplayFunc(displayCallback);
  glutReshapeFunc(reshapeCallback);
  glutKeyboardFunc(keyboardCallback);
  glutMouseFunc(mouseCallback);
  glutTimerFunc(SCENE_UPDATE_TIME, timerCallback, 0);

  if(!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
    pgr::dieWithError("pgr init failed, required OpenGL not supported?");

  init();

  std::cout << "USAGE: use the left and right mouse button to start/stop rotation of a cube" << std::endl;

  glutMainLoop();
  return 0;
}
