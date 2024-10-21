#include "triangle.h"
#include "stdint.h"
#include "display.h"

void int_swap(int *a, int *b)
{
    int storage = *a; // storage is whatever a points to
    *a = *b;          // whatever a points at is overriden by whatever b points to
    *b = storage;     // whatever b points to is overriden by storage
}

// Fills triangle with flat bottom
//       (x0, y0)
//        /    \
//       /      \
//      /        \
//     /          \
// (x1, y1)-----(x2, y2)

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Find inverse of slope(Otherwise we would be dividing by the slope in the for loop)
    float inverse_slope1 = (float)(x1 - x0) / (y1 - y0);
    float inverse_slope2 = (float)(x2 - x0) / (y2 - y0);

    float new_x1 = x0;
    float new_x2 = x0;
    // If line count is lower than expected (lower than 2? print hte inputs)
    for (int y = y0; y <= y2; y++)
    {
        draw_line(new_x1, y, new_x2, y, color);
        new_x1 += inverse_slope1;
        new_x2 += inverse_slope2;
    }
}

// Fills triangle with flat top
// (x0, y0)------(x1,y1)
//        \      /
//         \    /
//          \  /
//         (x2, y2)
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float inverse_slope_0 = (float)(x2 - x0) / (y2 - y0);
    float inverse_slope_1 = (float)(x2 - x1) / (y2 - y1);

    float new_x0 = x0;
    float new_x1 = x1;

    for (int y = y0; y < y2; y++)
    {
        draw_line(new_x0, y, new_x1, y, color);
        new_x0 += inverse_slope_0;
        new_x1 += inverse_slope_1;
    }
}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Enforce points are sorted in ascending order: y0<y1<y2
    if (y0 > y1)
    {
        int_swap(&x0, &x1);
        int_swap(&y0, &y1);
    }
    if (y1 > y2)
    {
        int_swap(&x1, &x2);
        int_swap(&y1, &y2);
    }
    if (y0 > y1)
    {
        int_swap(&x0, &x1);
        int_swap(&y0, &y1);
    }

    if (y1 == y2)
    { // triangle already has flat bottom, no need to draw the flat top
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else if (y0 == y1)
    { // tirangle has already flat top
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else
    {
        int mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;
        // int mx = (float)((y1 - y0) * (x2 - x0)) / (float)(y2 - y0) + x0;
        // Fill flat bottom triangle
        fill_flat_bottom_triangle(x0, y0, x1, y1, mx, y1, color);
        // Fill flat top triangle
        fill_flat_top_triangle(x1, y1, mx, y1, x2, y2, 0xFF889900);
    }
};
