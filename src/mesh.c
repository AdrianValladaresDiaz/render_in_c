#include <stdio.h>
#include "stdlib.h"
#include "ctype.h"
#include <string.h>
#include "stdbool.h"
#include "array.h"
#include "mesh.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0, 0, 0},
};

// load .obj into mesh
void load_obj_data(char *file_name)
{
    printf("Loading obj data.....");

    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("COULDNT OPEN FILE\n");
        return;
    }
    printf("opened obj file, loading data...\n");
    int MAX_LINE_LEN = 512;
    char buf[MAX_LINE_LEN];

    while (fgets(buf, MAX_LINE_LEN, file))
    {

        if (strncmp(buf, "v ", 2) == 0)
        {
            array_push(mesh.vertices, parse_vertex_line(buf));
        }
        else if (strncmp(buf, "f ", 2) == 0)
        {
            array_push(mesh.faces, parse_face_line(buf));
        }
    }
    fclose(file);
    printf("[OK]\n");
}

vec3_t parse_vertex_line(char *line)
{
    vec3_t vertex;
    sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
    return vertex;
}

face_t parse_face_line(char *line)
{
    face_t face;
    int vertex_indices[3];
    int texture_indices[3];
    int normal_indices[3];
    sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
           &vertex_indices[0], &texture_indices[0], &normal_indices[0],
           &vertex_indices[1], &texture_indices[1], &normal_indices[1],
           &vertex_indices[2], &texture_indices[2], &normal_indices[2]);

    face.a = vertex_indices[0];
    face.b = vertex_indices[1];
    face.c = vertex_indices[2];

    return face;
}