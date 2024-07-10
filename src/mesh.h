#ifndef MESH_H
#define MESH_H
#include "vector.h"
#include "triangle.h"

#define N_MESH_VERTICES 8
// Reminder exter means "defined externally"
extern vec3_t mesh_vertices[N_MESH_VERTICES];

#define N_MESH_FACES (6 * 4) // 6 cuve faces, 2 trieangles per face
extern face_t mesh_faces[N_MESH_FACES];

#endif