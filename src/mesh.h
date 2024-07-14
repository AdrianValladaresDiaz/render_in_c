#ifndef MESH_H
#define MESH_H
#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
// Reminder exter means "defined externally"
extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 trieangles per face
extern face_t cube_faces[N_CUBE_FACES];

typedef struct
{
    vec3_t *vertices; // Dynamic array of vertices
    face_t *faces;    // Dynamic array of faces
    vec3_t rotation;  // x,y,z values of the rotation
} mesh_t;

extern mesh_t mesh;
void load_cube_mesh_data(void);

#endif