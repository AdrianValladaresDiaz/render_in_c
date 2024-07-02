#ifndef DISPLAY_H
#define DISPLAY_H

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
void draw_rectangle(int start_x, int start_y, int end_x, int end_y, uint32_t color);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);

#endif