# Diary of a PGR student
## What it is
My walkthrought of the university subject computer graphic. Main challenges are the semestral project (opengl) and the final exam.
## Motivation
I am not very interested in computer graphic, that's why I created this repozitory. I usualy use private repository to backup my school project. This subject is special for me because it's boring and hard in the same time for me. I want to point out that teacher is very good. It's my personal choice to don't like computer graphics staff. :D What makes this walkthrought special is also that if I don't make it I will drop the school (by rules of university).
1. Make notes.
2. By explanation I will make sure that I understand the topic completely.
3. Can potentialy help others in future.
4. What is public usualy makes pressure for perfection.

## Knowladge base
I will describe list of topics. If topic is advance I will go into more details. Basics are basics. I won't waste much time on them.
### 1) Week
- Good resource for users who "know but don't remember" is [this](https://www.khronos.org/files/opengl-quick-reference-card.pdf) cheatsheet (specific for version, be careful).
#### **CPU Role in Compute Graphic**
- C++ (depending on project ofc)
- **OpenGL**:
  - Multiplatform independent standard.
  - Set of tools for rendering of 3D data and textures in to a 2D image
  - prepares block of data for passing to GPU
  - enable/disable particular functions
  - works like state machine: setup different things (that are read and rendered)
- Prepares the data for GPU (shaders).
- Sends everything to the graphics card.
- Sets the OpenGL state.
- Sends the “draw” commands.
- Handles user interaction (keyboard, mouse).
- Controls animations.
#### **GPU Role in Compute Graphic**
- GLSL
- Accepts data from the host.
- 60x per second
  - Clears the image memory
  - „draws a new image“
- Works in parallel
  - Vertices, Primitives, fragments
  - Runs shaders for each of them
#### **Framebuffer Role in Compute Graphic**
- Framebuffer = memory for the image
- Stores the rendered 2D image
- Has more layers: Color, Depth, ...
- Image is sent to the monitor
- 60x per second

#### **Bottleneck** is limited bandwidth
- Between graphic card and monitor. => CPU send data for precomputing; Significant portion of data is discarded.

#### **Rasterization Pipeline**
- Just learn it in detail.

#### Vectors
- Vector operations (sum, scale etc.)
- Vector lenght - the distance from the starting point to the ending point.
- Unit vector - `vector` is *unit vector* when `len(vector) = 1`
- Vector normalization - vector / len(vector) = normalized_vector. Note: `len(normalized_vector) = 1`, normalized vector is unit. When normalizing, vector has to have len > 0.
#### GLSL basics
- Colors have values from range <0.0f,1.0f>.
- linear interpolation *"lerp"*
- built-in functions
- in, out, uniform
- How to compile shader and how to use it.
  - VS typicaly modifies position of vertex and also gives color to FS.
  - FS out is then final color of the pixel.
  - `in` are variables that goes into shader. We set them in C/C++ program.
  - `out` are vars that goes out of shader.
  - uniform are variables that has same value for all shaders. Typical example can be rotation matrix. We set them in C/C++ program.
- *swizzling* - for example if i want to swap `vec.x` and `vec.z` I can do `vec = vec.zyx`
- I may describe how to use shader in opengl by example in the future. Maybe I will copy and comment parts of code that I will write in the future.

### 2) Week

#### recap of linear algebra
- **Rotation** matrix (MX) (cheat sheet bellow)
![image](https://user-images.githubusercontent.com/69775422/222754955-a7eb850f-e41d-4673-8299-635ce212c32c.png)
- **Transformation** MX and **Translation** MX (cheat sheet bellow): middle one is translation matrix. The last one is transformation matrix.
![image](https://user-images.githubusercontent.com/69775422/222755585-8dcc59a5-f5b5-4290-b936-e817906c950c.png)
- Vector space
- Linear independence
- Basis, [change of basis](https://youtu.be/P2LTAUO1TdA)

#### Shaders
- 'uniform' - values that can be modified from codebase (glUniform...). Uniforms can be accessed from both VS, FS
- Between VS and FS there is a interpolation. So basicaly exact positions are transformed (interpolated) into fragments.
##### 1. Vertex Shader (VS)
- 'in' - inputs for buffer values (vert. positions, normals, tex. coords)
- 'out' - binded to 'in's for FS
##### 2. Vertex Shader (FS)
- 'in' - binded to 'out's of VS
- output is fragment data

#### Buffers
- vertex array – format of the vertex data and connection to VS inputs
- action workflow:
  1. glGenBuffer
  2. glBindBuffer
  3. glBufferData
  ...
  n. glDeleteBuffer (when app finished: delete reference to the buffer)
- see example, it's clearer: [semestral work - buffers example](#buffers---object-loading-from-source)

#### Drawing
- `glDrawArrays(array_ptr, count)` draw a linear sequence of 𝑛 vertices
- `glDrawElements(...)` draw a random sequence of 𝑛 vertices according to the indices; (for arguments see official docs, or find examples)

#### Primitives
- opengl uses right handed coord sys
- height is `y` axis
- left, down: 0,0 (origin); right, top: (width, height) (max)
- primitives:
  - points
  - lines
  - line strip
  - line loop
  - triangles
  - triangle strip
  - triangle fan
#### Face orientation (inc. back face culling)
- whether face is draw when indexed clockwise (CC) or counterCW (CCW)
- after `glEnable(GL_CULL_FACE);` all the faces that are not front-faces are discarded  

### 2) Week
#### Objects
- Creation
  - generate object name → GLuint `glGenXXX(..., &objectName)`
  1. bind object for initialization – glBindXXX (target, objectName)
  2. copy data into the object `glBufferData()`, `glTexImage2D()`, ...
  3. + set OpenGL state
- Usage: `glBind(...)`
- Removement: `glDelete(...)`

### 2) Week
#### MVP Matrixes
- Input are model coordinates
1. model matrix: model coords -> world
2. view matrix: world coords -> camera
   - "eye"(=camera) coords
3. projection matrix: make perspective projection
   - cliping
4. perspective division "1/w" (4th coordinate)
   - normalized device coords
5. viewport transformation "Vp"
   - window coords
- Output are window coordinates

#### Transformation matrixes
- see how the operations affect the matrixes
- SRT Order (to don't mess things up): scale, rotation, transition
- rotation: axis matrix coord remains equal to one
  - e.g.: rotate around x-axis the coords of M[0,0] = 1 in a rotation matrix

#### View Matrix V/E^(-1)
- LookAt creates the view matrix
  - Eye (-Front??) = position of the viewpoint
  - Center (camera position) = any point along the desired line of sight (center, as it projects to the central screen pixel)
  - Up vector
  ```cpp
  vec3 z = -normalize(front);
  if (isVectorNull(z)) z = vec3(0.0, 0.0, 1.0);

  vec3 x = normalize(cross(up, z));
  if (isVectorNull(x)) x = vec3(1.0, 0.0, 0.0);

  vec3 y = cross(z, x);

  mat4 matrix = mat4(
    x.x, x.y, x.z, 0.0,                     // column 1
    y.x, y.y, y.z, 0.0,                     // column 2
    z.x, z.y, z.z, 0.0,                     // column 3
    position.x, position.y, position.z, 1.0 // column 4
  );
  return matrix; // need to inverse
  ```

#### **Exam Topics**
##### W2
- Passing vertex data to GPU
  - attribute variables, uniforms
  - buffers (VBO, VAO, ...)
- Buffer organization
  - geometry as sequence x indexed vertices
  - order of vertex attributes in buffers (sequential block, interleaved)
- Drawing loop
- Primitives
- Back face culling

##### W3
Fundamental concepts (point, vector, bases, coordinate frames, coordinates)
Matrices representing transformations
- Recognize the matrix type (linear, affine, projection)
- Derive/write matrices for: 𝑅𝑥, 𝑅𝑦 , 𝑅𝑧 , 𝑆, 𝑇, 𝑉𝑝, 𝑃 (explain 1/𝑤)
reference frames (change of: lookAt, on curve;
transf. with respect to: axes, virtual trackball)
Combination of transformations
- Order of matrix multiplications, decomposition of 𝑇𝐿
- Order of transformations in OpenGL
Limitations of matrices: gimbal lock, interpolation of rotations

#### **Random notes**
- shared normals => smooth edges (rounding); individual => sharp edges (no rounding)


## Semestral work
### Bindings
- **F1** - switch to free camera
- **F2** - switch to 1st static camera
- **F3** - switch to 2nd static camera
- **ARROWS** - free camera movement
- **MOUSE MOVEMENT** - free camera rotation
  
### Buffers - Object loading (from source)
One of task required is to load object from source, that has at least 30 triangles. I want to demostrate role of different buffers and how to use them.

Let me introduce my case. We have vertex array object (VAO), vertex buffer object (VBO) and element buffer object (EBO).
Formal definition and role of this buffers can be found around the internet everywhere. I will try to give newbie perspective:
1. VAO - Just pointer to you ordered data, prepared to be displayed.
2. VBO - Raw data. You will give vertexes with all attributes to VBO. VBO will just eat them.
3. EBO - How data are indexed. In my case i have indexes of triangles.

Note: geometry->vertexArrayObject, geometry->vertexBufferObject, geometry->elementBufferObject are just numbers (int). They act like pointers.
```cpp
	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereTriangles), sphereTriangles, GL_STATIC_DRAW);
```
**My object (sphere): **
As you can see bellow, every vertex has 8 atributes:
1. Position X (float)
2. Position Y (float)
3. Position Z (float)
4. Normal X (float)
5. Normal Y (float)
6. Normal Z (float)
7. Texture Coordinate U (float)
8. Texture Coordinate V (float)
```cpp
static constexpr float vertices[] = {
	  0.0f, 1.0f, -4.37114e-08f, 0.357407f, 0.862856f, -0.357407f, 0.75f,0.5f,
	  0.5f,0.5f,-0.707107f,0.357407f,0.862856f,-0.357407f,0.625f,0.25f,
	  0.0f,0.707107f,-0.707107f,0.357407f,0.862856f,-0.357407f,0.75f,0.25f,
    ...
}
const unsigned sphereTriangles[] = {
  0, 1, 2,
  3, 4, 5,
  6, 7, 8,
  ...
}

const int sphereNAttribsPerVertex = 8;
	const int sphereNVertices = 112;
	const int sphereNTriangles = 48;
```
Now i have to "say" to EBO, what data in VBO mean. Than I will use VAO to draw.
```cpp
// Position attribute
// - just idx. Start at 0 and increment on  other atributes
// - takes 3 spaces
// - data type is float
// - DEFAULT opt. do't bother
// - how long spaces between data are: 0 + 8 * 'size of float' will get me to second position attribute
// - starts at 0 idx
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sphereNAttribsPerVertex * sizeof(float), (void*)0);

// Normal attribute
// - just idx. Start at 0 and increment on  other atributes. This is second attribute, so idx 1.
// - takes 3 spaces
// - data type is float
// - DEFAULT opt. do't bother
// - how long spaces between data are: 0 + 8 * 'size of float' will get me to second position attribute
// - starts at 3+sizeof(float) (pointer aritmetics shifts me 6 float forward in array)
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sphereNAttribsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));

// Texture coordinate attribute
// - just idx. Start at 0 and increment on  other atributes. This is third attribute, so idx 1.
// - takes 2 spaces (because as you can see above, texture coords are just 2D)
// - data type is float
// - DEFAULT opt. do't bother
// - how long spaces between data are: 0 + 8 * 'size of float' will get me to second position attribute
// - starts at 6+sizeof(float) (pointer aritmetics shifts me 6 float forward in array)
glEnableVertexAttribArray(2);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sphereNAttribsPerVertex * sizeof(float), (void*)(6 * sizeof(float)));

// unbind
glBindVertexArray(0);
```
And now just draw. Use VAO pointer inicialized before.
```cpp
glBindVertexArray(geometry->vertexArrayObject);
glDrawElements(GL_TRIANGLES, 48*3, GL_UNSIGNED_INT, 0); // 48 (= number of triangles that object has) * 3 (=number of vertexes that triangle has)
glBindVertexArray(0); // unbind
```
See file sphere.cpp for more details.

## Practices
### 1) Hello World
This is just simple hello word which draws single triagle. Main purpose is that many small things have to be adjusted before projects can run. These tweaks can be found [here](https://cent.felk.cvut.cz/courses/PGR/). But in general this configuration is specific (yours will be different).
### 2) Shaders
Focused on GLSL - how to use `in`, `out` and `uniform` variables.
- How to bind `uniform` in cpp program.
  - (1) **.vert file:** Define in and uniform vars in your .vert file
```glsl
#version 140
in vec3 position;

uniform mat4  mPVM;   // transformation matrix
uniform int iTask;     // task number
```
  - (2.1) **cpp file:** Define `GLint` varibles. In these references to VS in and uniform will be stored like so:
  ```cpp
  struct Locations {
    // attributes
    GLint position;
    // uniforms
    GLint PVMMatrix;
    GLint task;  // task number
  } locations;
```
  - (2.2) **cpp file:** After creating shaders, get locations of attributes (see bellow). Reference to where to pass the in, uniform is now stored in locations.XXX variables.
  ```cpp
  // locations to shader input(s)
  // VS attribute
  locations.position = glGetAttribLocation(resources.program, "position");

  // uniforms
  locations.PVMMatrix = glGetUniformLocation(resources.program, "mPVM");
  locations.task      = glGetUniformLocation(resources.program, "iTask");
  ```
  - (2.2) **cpp file:** When needed set in/uniform like so. Use glUniform'N'TYPE'(GLint location, 'TYPE' value);
  ```cpp
  // '1i' stands for 'one integer'
  glUniform1i( locations.task, 8 );
  // to pass different datatype see documentation (and use brain)  
  ```
  
- Passing data from VS to FS (in, out variables). I won't go into detailes, because it's simple. Create `out` var in VS. Create `in` of same datatype in FS. These will be automaticaly binded. This way you can pass var from VS to FS with EZ.

## Abbreviations
VS - Vertex Shader
FS - Fragment Shader
MX - Matrix
var(s) - variables

## Credits
I believe that all materials I will be sharing in this repository can be found publicaly at https://cent.felk.cvut.cz/courses/PGR/ at the time that I am making the repo. Materials which are not from this site are created / adjusted by me. Otherwise I am linking the source.
