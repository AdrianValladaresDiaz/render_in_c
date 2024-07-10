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

void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;
    int side_length = delta_x > delta_y ? abs(delta_x) : abs(delta_y);

    // find how much to increment x and y each time
    float x_increment = delta_x / (float)side_length; // casted to float otherwise C returns an int
    float y_increment = delta_y / (float)side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= side_length; i++)
    {
        draw_pixel(current_x, current_y, color);
        current_x += x_increment;
        current_y += y_increment;
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

void draw_pixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    {

        COLOR_BUFFER[(WINDOW_WIDTH * y) + x] = color;
    }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void draw_rectangle(int start_x, int start_y, int width, int height, uint32_t color)
{
    for (int x = start_x; x < (width + start_x); x++)
    {
        for (int y = start_y; y < (height + start_y); y++)
        {
            draw_pixel(x, y, color);
        }
    }
}
