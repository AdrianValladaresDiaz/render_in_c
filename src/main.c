#include <stdio.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"

#define N_POINTS (9 * 9 * 9)

int previous_frame_time = 0;

triangle_t triangles_to_render[N_MESH_FACES];
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];
float FOV_FACTOR = 120;
vec3_t camera_position = {.x = 0, .y = 0, .z = 2.5};
vec3_t cube_rotation = {0, 0, 0};
uint32_t drawing_color = 0xFFFFFFFF;

void setup(void)
{
    // Alocate memory for Color buffer, malloc returns pointer to first position.
    COLOR_BUFFER = malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));
    if (!COLOR_BUFFER)
    {
        fprintf(stderr, "Could not allocate memory for [COLOR_BUFFER]");
    }

    COLOR_BUFFER_TEXTURE = SDL_CreateTexture(
        RENDERER,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_SetRenderDrawColor(RENDERER, 255, 0, 0, 255);
    SDL_RenderClear(RENDERER);
    int point_count = 0;
    for (float x = -1; x <= 1; x += 0.25)
    {
        for (float y = -1; y <= 1; y += 0.25)
        {
            for (float z = -1; z <= 1; z += 0.25)
            {
                vec3_t new_point = {x, y, z};
                cube_points[point_count++] = new_point;
            }
        }
    }
};

void process_input(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        fprintf(stdout, "%s\n", SDL_GetKeyName(event.key.keysym.sym));
        switch (event.type)
        {
        case SDL_QUIT:
            IS_RUNNING = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_a)
            {
                IS_RUNNING = false;
            }
            if (event.key.keysym.sym == SDLK_DOWN)
                drawing_color = 0xFFFF0000;
            if (event.key.keysym.sym == SDLK_UP)
                drawing_color = 0xFF00FF00;
            if (event.key.keysym.sym == SDLK_LEFT)
                drawing_color = 0xFF0000FF;
            if (event.key.keysym.sym == SDLK_DOWN)
                drawing_color = 0xFFFF0000;
            break;
        default:
            break;
        }
    }
};

vec2_t project_vec3_to_vec2(vec3_t point)
{
    vec2_t projection = {.x = point.x * FOV_FACTOR / point.z,
                         .y = point.y * FOV_FACTOR / point.z};
    return projection;
}

void update(void)
{
    // How much time needs to pass to the time between execuions is FRAME_TARGET_TIME?
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    // Only delay executuion if the program is running too fast.
    if (time_to_wait > 0)
    {
        SDL_Delay(time_to_wait);
    }
    previous_frame_time = SDL_GetTicks();

    triangle_t projected_triangle;
    // loop over all cube triangle faces
    for (int i = 0; i < N_MESH_FACES; i++)
    {
        face_t mesh_face = mesh_faces[i];
        // get vertices (points) that construe the face
        vec3_t face_vertices[3] = {
            mesh_vertices[mesh_face.a - 1], // mesh faces are NOT 0-indexed, start at 1
            mesh_vertices[mesh_face.b - 1],
            mesh_vertices[mesh_face.c - 1]};

        // Apply transforms to each face
        for (int k = 0; k < 3; k++)
        {
            vec3_t transformed_vertex = face_vertices[k];
            transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

            // Move vertex away from screen
            transformed_vertex.z -= camera_position.z;

            // Project current vertes as a triangle
            vec2_t projected_point = project_vec3_to_vec2(transformed_vertex);

            // Scale and translate to middle of screen
            projected_point.x += WINDOW_WIDTH / 2;
            projected_point.y += WINDOW_HEIGHT / 2;
            projected_triangle.points[k] = projected_point;
        }
        triangles_to_render[i] = projected_triangle;
    };

    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;
};

void render(void)
{
    // Set screen to RED, on start the memory may be dirty from older memory allocations.
    // draw_grid(0x10FF0000);

    for (int i = 0; i < N_MESH_FACES; i++)
    {
        triangle_t triangle = triangles_to_render[i];
        draw_rectangle(triangle.points[0].x, triangle.points[0].y, 3, 3, drawing_color);
        draw_rectangle(triangle.points[1].x, triangle.points[1].y, 3, 3, drawing_color);
        draw_rectangle(triangle.points[2].x, triangle.points[2].y, 3, 3, drawing_color);
    }

    render_color_buffer();          // Put on screen whatever is in the color_buffer -> texture -> renderer.
    clear_color_buffer(0x00000000); // Set entire buffer to yellow as default
    SDL_RenderPresent(RENDERER);
}

int main(void)
{

    IS_RUNNING = initialize_window();
    setup();
    while (IS_RUNNING)
    {
        process_input();
        update();
        render();
    }
    destroy_window();
    return 0;
}