#pragma once

// WINDOW ----------------------------------------------------------------------------------
constexpr char WINDOW_TITLE[] = "PGR: janskdus";
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;


// CAMERAS ---------------------------------------------------------------------------------
// Camera 1 (free camera)
#define CAMERA_FREE_SPEED 3.0f
#define CAMERA_FREE_ELEVATION_MAX 45.0f

#define CAMERA_FREE_INIT_POSITION glm::vec3(0.0f, 0.0f, 0.0f)

// Camera 2 - static
#define CAMERA_2_INIT_POSITION glm::vec3(0.0f, 0.0f, 0.0f)
#define CAMERA_2_INIT_DIRECTION glm::vec3(1.0f, 0.0f, 0.0f) // normalized in camera

// Camera 3 - static
#define CAMERA_3_INIT_POSITION glm::vec3(-9.0f, 0.0f, 7.5f)
#define CAMERA_3_INIT_DIRECTION glm::vec3(-0.75f, 0.0f, 0.65f) // normalized in camera


// MOVING OBJECT ----------------------------------------------------------------------------
#define MOVING_OBJECT_RADIUS 15.0f
#define MOVING_OBJECT_SPEED 1.0f


// TETRAHEDRON - vertex distance range from center ------------------------------------------ 
#define TETRAHEDRON_MIN_DIST 0.2f
#define TETRAHEDRON_MAX_DIST 1.2f


// ASTEROIDS --------------------------------------------------------------------------------
// grid 
#define ASTEROIDS_Z_MIN -40
#define ASTEROIDS_Z_MAX 40
#define ASTEROIDS_Z_COUNT 5
#define ASTEROIDS_Z_STEP abs(ASTEROIDS_Z_MIN - ASTEROIDS_Z_MAX) / ASTEROIDS_Z_COUNT

#define ASTEROIDS_X_MIN -60
#define ASTEROIDS_X_MAX -35
#define ASTEROIDS_X_COUNT 10
#define ASTEROIDS_X_STEP abs(ASTEROIDS_X_MIN - ASTEROIDS_X_MAX) / ASTEROIDS_X_COUNT

// random displacement adjustment
#define ASTEROIDS_MIN_DISPLACEMENT_HORIZONTAL 1.0f
#define ASTEROIDS_MAX_DISPLACEMENT_HORIZONTAL 3.0f
#define ASTEROIDS_MIN_DISPLACEMENT_VERTICAL -15.0f
#define ASTEROIDS_MAX_DISPLACEMENT_VERTICAL 15.0f

// random scale ratio (all axis)
#define ASTEROIDS_SCALE_MIN 0.2f
#define ASTEROIDS_SCALE_MAX 4.2f

// random rotation range (all axis)
#define ASTEROIDS_ANGLE_MIN 0.0f
#define ASTEROIDS_ANGLE_MAX 180.0f