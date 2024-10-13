#include "vector.h"
#include <math.h>

//-------------------------------------------------------------
//-------------------VEC_2 IMPLEMENTATIONS --------------------
//-------------------------------------------------------------

float vec2_length(vec2_t vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
};

vec2_t vec2_add(vec2_t a, vec2_t b)
{
    vec2_t v = {
        .x = a.x + b.x,
        .y = a.y + b.y};
    return v;
};

vec2_t vec2_sub(vec2_t a, vec2_t b)
{
    vec2_t v = {
        .x = a.x - b.x,
        .y = a.y - b.y};
    return v;
};

vec2_t vec2_mul(vec2_t vec, float factor)
{
    vec2_t scaled = {
        .x = vec.x * factor,
        .y = vec.y * factor,
    };
    return scaled;
};

vec2_t vec2_div(vec2_t vec, float factor)
{
    vec2_t scaled = {
        .x = vec.x / factor,
        .y = vec.y / factor,
    };
    return scaled;
};

float vec2_dot(vec2_t a, vec2_t b)
{
    return (a.x * b.x) + (a.y * b.y);
};

//-------------------------------------------------------------
//-------------------VEC_3 IMPLEMENTATIONS --------------------
//-------------------------------------------------------------

float vec3_length(vec3_t vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
};

vec3_t vec3_add(vec3_t a, vec3_t b)
{
    vec3_t v = {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z};
    return v;
};

vec3_t vec3_sub(vec3_t a, vec3_t b)
{
    vec3_t v = {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z};
    return v;
};

vec3_t vec3_rotate_x(vec3_t vec, float angle)
{
    vec3_t rotated = {
        .x = vec.x,
        .y = vec.y * cos(angle) - vec.z * sin(angle),
        .z = vec.z * cos(angle) + vec.y * sin(angle)};
    return rotated;
}

vec3_t vec3_rotate_y(vec3_t vec, float angle)
{
    vec3_t rotated = {
        .x = vec.x * cos(angle) - vec.z * sin(angle),
        .y = vec.y,
        .z = vec.z * cos(angle) + vec.x * sin(angle)};
    return rotated;
}

vec3_t vec3_rotate_z(vec3_t vec, float angle)
{
    vec3_t rotated = {
        .x = vec.x * cos(angle) - vec.y * sin(angle),
        .y = vec.y * cos(angle) + vec.x * sin(angle),
        .z = vec.z};
    return rotated;
}

vec3_t vec3_mul(vec3_t vec, float factor)
{
    vec3_t scaled = {
        .x = vec.x * factor,
        .y = vec.y * factor,
        .z = vec.z * factor,
    };
    return scaled;
};

vec3_t vec3_div(vec3_t vec, float factor)
{
    vec3_t scaled = {
        .x = vec.x / factor,
        .y = vec.y / factor,
        .z = vec.z / factor,
    };
    return scaled;
};

vec3_t vec3_cross(vec3_t a, vec3_t b)
{
    vec3_t result = {
        .x = (a.y * b.z) - (a.z * b.y),
        .y = (a.z * b.x) - (a.x * b.z),
        .z = (a.x * b.y) - (a.y * b.x),
    };
    return result;
};

float vec3_dot(vec3_t a, vec3_t b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
};
