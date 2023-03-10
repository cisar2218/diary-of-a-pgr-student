// GENERATED BY jsmodel2c, DO NOT EDIT DIRECTLY
// model was downloaded from http://www.ro.me/files/models/

#pragma once

struct birds_data_t {
  int nFaces; // we use only triangles, each triangle is 3xshort int
  const unsigned short * faces;
  int nVertices; // we store each vertex as 3xfloat for each frame
  const float * vertices;
  const float color[3];
  int nAnimFrames; // it will be always at least 1 (static model)
};

#ifdef __cplusplus
extern "C" const struct birds_data_t birds_data;
#else
extern const struct birds_data_t birds_data;
#endif

