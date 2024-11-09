#ifndef DISPLAY_H
#define DISPLAY_H
#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

// GLOBALS
extern SDL_Window *WINDOW;
extern SDL_Renderer *RENDERER;
extern bool IS_RUNNING;
extern uint32_t *COLOR_BUFFER;
extern SDL_Texture *COLOR_BUFFER_TEXTURE;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

bool initialize_window(void);
void destroy_window();
void draw_grid(uint32_t color);
void draw_pixel(int x, int y, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_rectangle(int start_x, int start_y, int width, int height, uint32_t color);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
// void draw_flat_bottom(x0, y0, x1, y1, mx, my);
// void draw_flat_top(x0, y0, mx, my, x1, y1);

#endif