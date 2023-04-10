#include "sphere.h"
const int sphereNAttribsPerVertex = 8;
const int sphereNVertices = 112;
const int sphereNTriangles = 48;
const float sphereVertices[] = {
  0.0f,1.0f,-4.37114e-08f,0.357407f,0.862856f,-0.357407f,0.75f,0.5f,
  0.5f,0.5f,-0.707107f,0.357407f,0.862856f,-0.357407f,0.625f,0.25f,
  0.0f,0.707107f,-0.707107f,0.357407f,0.862856f,-0.357407f,0.75f,0.25f,
  0.0f,0.707107f,0.707107f,0.156558f,0.377964f,0.912487f,0.75f,0.75f,
  -6.18172e-08f,-1.4924e-07f,1.0f,0.156558f,0.377964f,0.912487f,0.6875f,1.0f,
  0.5f,0.5f,0.707107f,0.156558f,0.377964f,0.912487f,0.625f,0.75f,
  0.0f,-8.74228e-08f,-1.0f,0.156558f,0.377964f,-0.912487f,0.6875f,0.0f,
  0.0f,0.707107f,-0.707107f,0.156558f,0.377964f,-0.912487f,0.75f,0.25f,
  0.5f,0.5f,-0.707107f,0.156558f,0.377964f,-0.912487f,0.625f,0.25f,
  0.0f,0.707107f,0.707107f,0.357407f,0.862856f,0.357407f,0.75f,0.75f,
  0.707107f,0.707107f,-4.37114e-08f,0.357407f,0.862856f,0.357407f,0.625f,0.5f,
  0.0f,1.0f,-4.37114e-08f,0.357407f,0.862856f,0.357407f,0.75f,0.5f,
  0.0f,-8.74228e-08f,-1.0f,0.377964f,0.156558f,-0.912487f,0.5625f,0.0f,
  0.5f,0.5f,-0.707107f,0.377964f,0.156558f,-0.912487f,0.625f,0.25f,
  0.707107f,-8.52102e-08f,-0.707107f,0.377964f,0.156558f,-0.912487f,0.5f,0.25f,
  0.5f,0.5f,0.707107f,0.862856f,0.357407f,0.357407f,0.625f,0.75f,
  1.0f,-5.54079e-08f,-4.37114e-08f,0.862856f,0.357407f,0.357407f,0.5f,0.5f,
  0.707107f,0.707107f,-4.37114e-08f,0.862856f,0.357407f,0.357407f,0.625f,0.5f,
  0.707107f,0.707107f,-4.37114e-08f,0.862856f,0.357407f,-0.357407f,0.625f,0.5f,
  0.707107f,-8.52102e-08f,-0.707107f,0.862856f,0.357407f,-0.357407f,0.5f,0.25f,
  0.5f,0.5f,-0.707107f,0.862856f,0.357407f,-0.357407f,0.625f,0.25f,
  0.5f,0.5f,0.707107f,0.377964f,0.156558f,0.912487f,0.625f,0.75f,
  -6.18172e-08f,-1.4924e-07f,1.0f,0.377964f,0.156558f,0.912487f,0.5625f,1.0f,
  0.707107f,-8.52102e-08f,0.707107f,0.377964f,0.156558f,0.912487f,0.5f,0.75f,
  0.707107f,-8.52102e-08f,0.707107f,0.377964f,-0.156558f,0.912487f,0.5f,0.75f,
  -6.18172e-08f,-1.4924e-07f,1.0f,0.377964f,-0.156558f,0.912487f,0.4375f,1.0f,
  0.5f,-0.5f,0.707107f,0.377964f,-0.156558f,0.912487f,0.375f,0.75f,
  0.0f,-8.74228e-08f,-1.0f,0.377964f,-0.156558f,-0.912487f,0.4375f,0.0f,
  0.707107f,-8.52102e-08f,-0.707107f,0.377964f,-0.156558f,-0.912487f,0.5f,0.25f,
  0.5f,-0.5f,-0.707107f,0.377964f,-0.156558f,-0.912487f,0.375f,0.25f,
  0.707107f,-8.52102e-08f,0.707107f,0.862856f,-0.357407f,0.357407f,0.5f,0.75f,
  0.707107f,-0.707107f,-4.37114e-08f,0.862856f,-0.357407f,0.357407f,0.375f,0.5f,
  1.0f,-5.54079e-08f,-4.37114e-08f,0.862856f,-0.357407f,0.357407f,0.5f,0.5f,
  1.0f,-5.54079e-08f,-4.37114e-08f,0.862856f,-0.357407f,-0.357407f,0.5f,0.5f,
  0.5f,-0.5f,-0.707107f,0.862856f,-0.357407f,-0.357407f,0.375f,0.25f,
  0.707107f,-8.52102e-08f,-0.707107f,0.862856f,-0.357407f,-0.357407f,0.5f,0.25f,
  0.707107f,-0.707107f,-4.37114e-08f,0.357407f,-0.862856f,-0.357407f,0.375f,0.5f,
  3.20149e-08f,-0.707107f,-0.707107f,0.357407f,-0.862856f,-0.357407f,0.25f,0.25f,
  0.5f,-0.5f,-0.707107f,0.357407f,-0.862856f,-0.357407f,0.375f,0.25f,
  0.5f,-0.5f,0.707107f,0.156558f,-0.377965f,0.912487f,0.375f,0.75f,
  -6.18172e-08f,-1.4924e-07f,1.0f,0.156558f,-0.377965f,0.912487f,0.3125f,1.0f,
  3.20149e-08f,-0.707107f,0.707107f,0.156558f,-0.377965f,0.912487f,0.25f,0.75f,
  0.0f,-8.74228e-08f,-1.0f,0.156558f,-0.377964f,-0.912487f,0.3125f,0.0f,
  0.5f,-0.5f,-0.707107f,0.156558f,-0.377964f,-0.912487f,0.375f,0.25f,
  3.20149e-08f,-0.707107f,-0.707107f,0.156558f,-0.377964f,-0.912487f,0.25f,0.25f,
  0.5f,-0.5f,0.707107f,0.357407f,-0.862856f,0.357407f,0.375f,0.75f,
  2.21259e-09f,-1.0f,-4.37114e-08f,0.357407f,-0.862856f,0.357407f,0.25f,0.5f,
  0.707107f,-0.707107f,-4.37114e-08f,0.357407f,-0.862856f,0.357407f,0.375f,0.5f,
  2.21259e-09f,-1.0f,-4.37114e-08f,-0.357407f,-0.862856f,-0.357407f,0.25f,0.5f,
  -0.5f,-0.5f,-0.707107f,-0.357407f,-0.862856f,-0.357407f,0.125f,0.25f,
  3.20149e-08f,-0.707107f,-0.707107f,-0.357407f,-0.862856f,-0.357407f,0.25f,0.25f,
  3.20149e-08f,-0.707107f,0.707107f,-0.156558f,-0.377965f,0.912487f,0.25f,0.75f,
  -6.18172e-08f,-1.4924e-07f,1.0f,-0.156558f,-0.377965f,0.912487f,0.1875f,1.0f,
  -0.5f,-0.5f,0.707107f,-0.156558f,-0.377965f,0.912487f,0.125f,0.75f,
  0.0f,-8.74228e-08f,-1.0f,-0.156558f,-0.377964f,-0.912487f,0.1875f,0.0f,
  3.20149e-08f,-0.707107f,-0.707107f,-0.156558f,-0.377964f,-0.912487f,0.25f,0.25f,
  -0.5f,-0.5f,-0.707107f,-0.156558f,-0.377964f,-0.912487f,0.125f,0.25f,
  3.20149e-08f,-0.707107f,0.707107f,-0.357407f,-0.862856f,0.357407f,0.25f,0.75f,
  -0.707107f,-0.707107f,-4.37114e-08f,-0.357407f,-0.862856f,0.357407f,0.125f,0.5f,
  2.21259e-09f,-1.0f,-4.37114e-08f,-0.357407f,-0.862856f,0.357407f,0.25f,0.5f,
  -0.5f,-0.5f,-0.707107f,-0.862856f,-0.357407f,-0.357407f,0.125f,0.25f,
  -1.0f,-5.54079e-08f,-4.37114e-08f,-0.862856f,-0.357407f,-0.357407f,0.0f,0.5f,
  -0.707107f,-8.52102e-08f,-0.707107f,-0.862856f,-0.357407f,-0.357407f,0.0f,0.25f,
  -0.5f,-0.5f,0.707107f,-0.377965f,-0.156558f,0.912487f,0.125f,0.75f,
  -6.18172e-08f,-1.4924e-07f,1.0f,-0.377965f,-0.156558f,0.912487f,0.0625f,1.0f,
  -0.707107f,-8.52102e-08f,0.707107f,-0.377965f,-0.156558f,0.912487f,0.0f,0.75f,
  0.0f,-8.74228e-08f,-1.0f,-0.377965f,-0.156558f,-0.912487f,0.0625f,0.0f,
  -0.5f,-0.5f,-0.707107f,-0.377965f,-0.156558f,-0.912487f,0.125f,0.25f,
  -0.707107f,-8.52102e-08f,-0.707107f,-0.377965f,-0.156558f,-0.912487f,0.0f,0.25f,
  -0.5f,-0.5f,0.707107f,-0.862856f,-0.357407f,0.357407f,0.125f,0.75f,
  -1.0f,-5.54079e-08f,-4.37114e-08f,-0.862856f,-0.357407f,0.357407f,0.0f,0.5f,
  -0.707107f,-0.707107f,-4.37114e-08f,-0.862856f,-0.357407f,0.357407f,0.125f,0.5f,
  -1.0f,-5.54079e-08f,-4.37114e-08f,-0.862856f,0.357407f,-0.357407f,1.0f,0.5f,
  -0.5f,0.5f,-0.707107f,-0.862856f,0.357407f,-0.357407f,0.875f,0.25f,
  -0.707107f,-8.52102e-08f,-0.707107f,-0.862856f,0.357407f,-0.357407f,1.0f,0.25f,
  -0.707107f,-8.52102e-08f,0.707107f,-0.377965f,0.156558f,0.912487f,1.0f,0.75f,
  -6.18172e-08f,-1.4924e-07f,1.0f,-0.377965f,0.156558f,0.912487f,0.9375f,1.0f,
  -0.5f,0.5f,0.707107f,-0.377965f,0.156558f,0.912487f,0.875f,0.75f,
  0.0f,-8.74228e-08f,-1.0f,-0.377965f,0.156558f,-0.912487f,0.9375f,0.0f,
  -0.707107f,-8.52102e-08f,-0.707107f,-0.377965f,0.156558f,-0.912487f,1.0f,0.25f,
  -0.5f,0.5f,-0.707107f,-0.377965f,0.156558f,-0.912487f,0.875f,0.25f,
  -0.707107f,-8.52102e-08f,0.707107f,-0.862856f,0.357407f,0.357407f,1.0f,0.75f,
  -0.707107f,0.707107f,-4.37114e-08f,-0.862856f,0.357407f,0.357407f,0.875f,0.5f,
  -1.0f,-5.54079e-08f,-4.37114e-08f,-0.862856f,0.357407f,0.357407f,1.0f,0.5f,
  -0.707107f,0.707107f,-4.37114e-08f,-0.357407f,0.862856f,-0.357407f,0.875f,0.5f,
  0.0f,0.707107f,-0.707107f,-0.357407f,0.862856f,-0.357407f,0.75f,0.25f,
  -0.5f,0.5f,-0.707107f,-0.357407f,0.862856f,-0.357407f,0.875f,0.25f,
  -0.5f,0.5f,0.707107f,-0.156558f,0.377964f,0.912487f,0.875f,0.75f,
  -6.18172e-08f,-1.4924e-07f,1.0f,-0.156558f,0.377964f,0.912487f,0.8125f,1.0f,
  0.0f,0.707107f,0.707107f,-0.156558f,0.377964f,0.912487f,0.75f,0.75f,
  0.0f,-8.74228e-08f,-1.0f,-0.156558f,0.377964f,-0.912487f,0.8125f,0.0f,
  -0.5f,0.5f,-0.707107f,-0.156558f,0.377964f,-0.912487f,0.875f,0.25f,
  0.0f,0.707107f,-0.707107f,-0.156558f,0.377964f,-0.912487f,0.75f,0.25f,
  -0.707107f,0.707107f,-4.37114e-08f,-0.357407f,0.862856f,0.357407f,0.875f,0.5f,
  0.0f,0.707107f,0.707107f,-0.357407f,0.862856f,0.357407f,0.75f,0.75f,
  0.0f,1.0f,-4.37114e-08f,-0.357407f,0.862856f,0.357407f,0.75f,0.5f,
  0.707107f,0.707107f,-4.37114e-08f,0.357407f,0.862856f,-0.357407f,0.625f,0.5f,
  0.5f,0.5f,0.707107f,0.357407f,0.862856f,0.357407f,0.625f,0.75f,
  0.707107f,-8.52102e-08f,0.707107f,0.862856f,0.357407f,0.357407f,0.5f,0.75f,
  1.0f,-5.54079e-08f,-4.37114e-08f,0.862856f,0.357407f,-0.357407f,0.5f,0.5f,
  0.5f,-0.5f,0.707107f,0.862856f,-0.357407f,0.357407f,0.375f,0.75f,
  0.707107f,-0.707107f,-4.37114e-08f,0.862856f,-0.357407f,-0.357407f,0.375f,0.5f,
  2.21259e-09f,-1.0f,-4.37114e-08f,0.357407f,-0.862856f,-0.357407f,0.25f,0.5f,
  3.20149e-08f,-0.707107f,0.707107f,0.357407f,-0.862856f,0.357407f,0.25f,0.75f,
  -0.707107f,-0.707107f,-4.37114e-08f,-0.357407f,-0.862856f,-0.357407f,0.125f,0.5f,
  -0.5f,-0.5f,0.707107f,-0.357407f,-0.862856f,0.357407f,0.125f,0.75f,
  -0.707107f,-0.707107f,-4.37114e-08f,-0.862856f,-0.357407f,-0.357407f,0.125f,0.5f,
  -0.707107f,-8.52102e-08f,0.707107f,-0.862856f,-0.357407f,0.357407f,0.0f,0.75f,
  -0.707107f,0.707107f,-4.37114e-08f,-0.862856f,0.357407f,-0.357407f,0.875f,0.5f,
  -0.5f,0.5f,0.707107f,-0.862856f,0.357407f,0.357407f,0.875f,0.75f,
  0.0f,1.0f,-4.37114e-08f,-0.357407f,0.862856f,-0.357407f,0.75f,0.5f,
  -0.5f,0.5f,0.707107f,-0.357407f,0.862856f,0.357407f,0.875f,0.75f,
}; // end sphereVertices

const unsigned sphereTriangles[] = {
  0, 1, 2,
  3, 4, 5,
  6, 7, 8,
  9, 10, 11,
  12, 13, 14,
  15, 16, 17,
  18, 19, 20,
  21, 22, 23,
  24, 25, 26,
  27, 28, 29,
  30, 31, 32,
  33, 34, 35,
  36, 37, 38,
  39, 40, 41,
  42, 43, 44,
  45, 46, 47,
  48, 49, 50,
  51, 52, 53,
  54, 55, 56,
  57, 58, 59,
  60, 61, 62,
  63, 64, 65,
  66, 67, 68,
  69, 70, 71,
  72, 73, 74,
  75, 76, 77,
  78, 79, 80,
  81, 82, 83,
  84, 85, 86,
  87, 88, 89,
  90, 91, 92,
  93, 94, 95,
  0, 96, 1,
  9, 97, 10,
  15, 98, 16,
  18, 99, 19,
  30, 100, 31,
  33, 101, 34,
  36, 102, 37,
  45, 103, 46,
  48, 104, 49,
  57, 105, 58,
  60, 106, 61,
  69, 107, 70,
  72, 108, 73,
  81, 109, 82,
  84, 110, 85,
  93, 111, 94,
}; // end sphereTriangles
