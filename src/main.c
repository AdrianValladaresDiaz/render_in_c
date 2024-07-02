#include <stdio.h>
#include "display.h"

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
                IS_RUNNING = false;
            break;
        default:
            break;
        }
    }
};

void update(void) {};

void render(void)
{
    // Set screen to RED, on start the memory may be dirty from older memory allocations.
    draw_grid(0xFFFF0000);
    draw_rectangle(20, 20, 400, 400, 0xFFFF0000);
    render_color_buffer();          // Put on screen whatever is in the color_buffer -> texture -> renderer.
    clear_color_buffer(0xFFFFFF00); // Set entire buffer to yellow as default
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