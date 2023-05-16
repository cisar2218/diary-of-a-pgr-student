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