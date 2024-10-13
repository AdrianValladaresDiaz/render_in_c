#include <stdio.h>
#include <unistd.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"

#define N_POINTS (9 * 9 * 9)

int previous_frame_time = 0;

triangle_t *triangles_to_render = NULL;
vec2_t projected_points[N_POINTS];
float FOV_FACTOR = 400;
vec3_t camera_position = {.x = 0, .y = 0, .z = 0};
uint32_t drawing_color = 0xFFFFFFFF;

void setup(void)
{
    printf("Starting setup\n");
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
    load_obj_data("./assets/f22.obj");
    printf("Setup Done\n");
};

void process_input(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            IS_RUNNING = false;
            break;
        case SDL_KEYDOWN:
            fprintf(stdout, "%s\n", SDL_GetKeyName(event.key.keysym.sym));
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

bool is_facing_back(vec3_t vertices[], vec3_t camera)
{
    vec3_t vector_a = vertices[0]; /*  A  */
    vec3_t vector_b = vertices[1]; /* / \ */
    vec3_t vector_c = vertices[2]; /*C   B*/

    vec3_t vector_ab = vec3_sub(vector_b, vector_a);
    vec3_t vector_ac = vec3_sub(vector_c, vector_a);
    /* left-handed coordinate system (so positive z is *closer* to the screen),
        index finger: AB
        middle finger: AC
        thumb: normal
    */
    vec3_t normal = vec3_cross(vector_ab, vector_ac);

    // find ray from A to camera
    vec3_t camera_ray = vec3_sub(camera, vector_a);

    // find alignment between camera ray and normal
    float alignment = vec3_dot(normal, camera_ray);

    if (alignment >= 0) // facing front
    {
        return false;
    }
    return true;
}

void update(void)
{
    // How much time needs to pass so the time between execuions is FRAME_TARGET_TIME?
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    // Only delay execution if the program is running too fast.
    if (time_to_wait > 0)
    {
        SDL_Delay(time_to_wait);
    }
    previous_frame_time = SDL_GetTicks();

    triangles_to_render = NULL; // reinitialize the triangle array
    mesh.rotation.y += 0.01;
    mesh.rotation.z += 0.01;

    // loop over all faces(triangles) in mesh
    for (int i = 0; i < array_length(mesh.faces); i++)
    {
        // get vertices (points) that construe the  current face
        vec3_t face_vertices[3] = {
            mesh.vertices[mesh.faces[i].a - 1], // mesh faces are NOT 0-indexed, start at 1
            mesh.vertices[mesh.faces[i].b - 1],
            mesh.vertices[mesh.faces[i].c - 1]};

        // Apply transforms to current face
        vec3_t transformed_vertices[3];
        for (int j = 0; j < 3; j++)
        {
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // Move vertex away from screen
            transformed_vertex.z += 2.5;

            // Save the transform
            transformed_vertices[j] = transformed_vertex;
        }

        // // Cull back-facing faces
        if (is_facing_back(transformed_vertices, camera_position) == true)
        {
            continue; // go to next face in the loop
        }

        // Project vertices onto screen plane
        triangle_t projected_triangle;
        for (int k = 0; k < 3; k++)
        {
            // Project current vertex as a triangle
            vec2_t projected_point = project_vec3_to_vec2(transformed_vertices[k]);

            // Scale and translate to middle of screen
            projected_point.x += WINDOW_WIDTH / 2;
            projected_point.y += WINDOW_HEIGHT / 2;
            projected_triangle.points[k] = projected_point;
        }
        array_push(triangles_to_render, projected_triangle);
    };
};

void render(void)
{
    // Set screen to RED, on start the memory may be dirty from older memory allocations.
    // draw_grid(0x10FF0000);

    for (int i = 0; i < array_length(triangles_to_render); i++)
    {
        triangle_t triangle = triangles_to_render[i];
        draw_rectangle(triangle.points[0].x, triangle.points[0].y, 3, 3, drawing_color);
        draw_rectangle(triangle.points[1].x, triangle.points[1].y, 3, 3, drawing_color);
        draw_rectangle(triangle.points[2].x, triangle.points[2].y, 3, 3, drawing_color);
        draw_triangle(
            triangle.points[0].x,
            triangle.points[0].y,
            triangle.points[1].x,
            triangle.points[1].y,
            triangle.points[2].x,
            triangle.points[2].y,
            drawing_color);
    }

    array_free(triangles_to_render); // Free the tringle array every frame
    render_color_buffer();           // Put on screen whatever is in the color_buffer -> texture -> renderer.
    clear_color_buffer(0x00000000);  // Set entire buffer to yellow as default
    SDL_RenderPresent(RENDERER);
}

// Frees dynamically allocated stuff
void free_resources(void)
{
    free(COLOR_BUFFER);
    array_free(mesh.faces);
    array_free(mesh.vertices);
}

int print_current_dir(void)
{
    char cwd[_PC_PATH_MAX + 500];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    else
    {
        perror("getcwd() error");
        return 1;
    }
    return 0;
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
    free_resources();
    return 0;
}