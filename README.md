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
var(s) - variables

## Credits
I believe that all materials I will be sharing in this repository can be found publicaly at https://cent.felk.cvut.cz/courses/PGR/ at the time that I am making the repo. Materials which are not from this site are created / adjusted by me. Otherwise I am linking the source.
