#include "display.h"

SDL_Window *WINDOW = NULL;
SDL_Renderer *RENDERER = NULL;
bool IS_RUNNING = NULL;
uint32_t *COLOR_BUFFER = NULL;
SDL_Texture *COLOR_BUFFER_TEXTURE = NULL;
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

/**
 * Sets up WINDOW and RENDERER
 */
bool initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL\n");
        return false;
    }

    // Use SDL to query the fullscreen max width and height of the monitor
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode); // puts current display info into "display mode pointer"
    WINDOW_HEIGHT = display_mode.h;
    WINDOW_WIDTH = display_mode.w;

    WINDOW = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS);
    if (!WINDOW)
    {
        fprintf(stderr, "Error creating SDL WINDOW\n");
        return false;
    }
    SDL_SetWindowFullscreen(WINDOW, SDL_WINDOW_FULLSCREEN);

    RENDERER = SDL_CreateRenderer(WINDOW, -1, 0);
    if (!RENDERER)
    {
        fprintf(stderr, "Error initializing SDL RENDERER");
        return false;
    }

    return true;
}

void destroy_window()
{
    free(COLOR_BUFFER);
    SDL_DestroyRenderer(RENDERER);
    SDL_DestroyWindow(WINDOW);
    SDL_DestroyTexture(COLOR_BUFFER_TEXTURE);
    SDL_Quit();
}

void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            COLOR_BUFFER[(WINDOW_WIDTH * y) + x] = color;
        }
    }
}

void draw_grid(uint32_t color)
{
    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            if (y % 10 == 0 || x % 10 == 0)
            {
                COLOR_BUFFER[(WINDOW_WIDTH * y) + x] = color;
            }
        }
    }
}

void render_color_buffer(void)
{
    // Put pixel buffer into the texture
    SDL_UpdateTexture(COLOR_BUFFER_TEXTURE, NULL, COLOR_BUFFER, WINDOW_WIDTH * sizeof(uint32_t));
    // Copy texture into renderer
    SDL_RenderCopy(RENDERER, COLOR_BUFFER_TEXTURE, NULL, NULL);
}

void draw_rectangle(int start_x, int start_y, int end_x, int end_y, uint32_t color)
{
    for (int x = start_x; x <= end_x; x++)
    {
        for (int y = start_y; y <= end_y; y++)
        {
            COLOR_BUFFER[(WINDOW_WIDTH * y) + x] = color;
        }
    }
}
