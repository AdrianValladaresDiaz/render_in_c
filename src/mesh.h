#ifndef MESH_H
#define MESH_H
#include "vector.h"
#include "triangle.h"

typedef struct
{
    vec3_t *vertices; // Dynamic array of vertices
    face_t *faces;    // Dynamic array of faces
    vec3_t rotation;  // x,y,z values of the rotation
} mesh_t;

extern mesh_t mesh;
void load_cube_mesh_data(void);
void load_obj_data(char *file_name);
vec3_t parse_vertex_line(char *line);
face_t parse_face_line(char *line);

#endif