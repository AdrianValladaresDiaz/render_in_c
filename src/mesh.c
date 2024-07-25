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

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 1
    {.x = -1, .y = 1, .z = -1},  // 2
    {.x = 1, .y = 1, .z = -1},   // 3
    {.x = 1, .y = -1, .z = -1},  // 4
    {.x = 1, .y = 1, .z = 1},    // 5
    {.x = 1, .y = -1, .z = 1},   // 6
    {.x = -1, .y = 1, .z = 1},   // 7
    {.x = -1, .y = -1, .z = 1},  // 8
};

// NOTE: The order of vertices in a face define what is the front of the face.
// Use the left hand rule to find direction. Face 1,2,3 points in the opposit direction 1,3,2 does.
face_t cube_faces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},
    // right
    {.a = 4, .b = 3, .c = 5},
    {.a = 4, .b = 5, .c = 6},
    // back
    {.a = 6, .b = 5, .c = 7},
    {.a = 6, .b = 7, .c = 8},
    // left
    {.a = 8, .b = 7, .c = 2},
    {.a = 8, .b = 2, .c = 1},
    // top
    {.a = 2, .b = 7, .c = 5},
    {.a = 2, .b = 5, .c = 3},
    // bottom
    {.a = 6, .b = 8, .c = 1},
    {.a = 6, .b = 1, .c = 4},
};

void load_cube_mesh_data(void)
{
    for (int i = 0; i < N_CUBE_VERTICES; i++)
    {
        array_push(mesh.vertices, cube_vertices[i]);
    }

    for (int i = 0; i < N_CUBE_FACES; i++)
    {
        array_push(mesh.faces, cube_faces[i]);
    }
}

// load .obj into mesh
void load_obj_data(char *file_name)
{

    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("COULDNT OPEN FILE\n");
        return;
    }
    printf("opened obj file, loading data...\n");
    int MAX_LINE_LEN = 40;
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
}

vec3_t parse_vertex_line(char *line)
{
    bool done = false;
    int string_length = 9;
    char string[string_length];
    int read_pos = 2;
    int string_insert_pos = 0;
    float parsed_floats[3] = {0., 0., 0.};
    int pf_insert_pos = 0;

    char current_char = line[read_pos];
    while (!done)
    {
        if (isspace(current_char)) // we will get a newline before the buffer ends.
        {
            parsed_floats[pf_insert_pos] = atof(string); // push result
            pf_insert_pos += 1;
            read_pos += 1; // advance indices
            current_char = line[read_pos];
            string_insert_pos = 0;
            for (int i = 0; i < string_length; i++) // clear string
            {
                string[i] = '0';
            }
            if (pf_insert_pos > 2) // done after third float value
            {
                done = true;
            }
        }
        else if (current_char == '\0')
        {
            parsed_floats[pf_insert_pos] = atof(string); // push result
            done = true;
        }
        else
        {
            string[string_insert_pos] = line[read_pos];
            string_insert_pos += 1;
            read_pos += 1;
            current_char = line[read_pos];
        }
    }
    vec3_t result = {
        .x = parsed_floats[0],
        .y = parsed_floats[1],
        .z = parsed_floats[2]};
    printf("vertex: %f %f %f\n", result.x, result.y, result.z);
    return result;
}

// optimize this, we know how many vertices tehre are before calling this
face_t parse_face_line(char *line)
{
    int faces[3];
    int face_insert_pos = 0;

    bool done = false;
    int read_pos = 1;
    int num_start_index = 2;
    int num_end_index = 2;
    int state = 0; // 0->reading number, 1->looking for next number
    while (!done)
    {
        read_pos += 1;
        if (state == 1)
        {
            if (isspace(line[read_pos]))
            {
                // found start of next vertex
                state = 0;
                num_start_index = read_pos;
                num_end_index = read_pos + 1;
            }
        }
        else // state is 0
        {
            if (!(line[read_pos] == '/'))
            {
                // keep reading
                num_end_index += 1;
            }
            else
            {
                state = 1; // look for next vertex position
                // finish reading current number
                char *face_string = malloc((sizeof(char)) * (num_end_index - num_start_index));
                // char face_string[num_end_index - num_start_index];
                int fs_insert_pos = 0;
                // printf("startend: %i, %i\n", num_start_index, num_end_index);
                for (int c = num_start_index; c < num_end_index; c++)
                {
                    face_string[fs_insert_pos] = line[c];
                    fs_insert_pos++;
                }
                faces[face_insert_pos] = atoi(face_string);
                free(face_string);
                face_insert_pos += 1;
                if (face_insert_pos >= 3)
                {
                    done = true;
                }
            }
        }
    }
    face_t result = {.a = faces[0], .b = faces[1], .c = faces[2]};
    printf("face %i,%i,%i\n", result.a, result.b, result.c);
    return result;
}