#include "vector.h"
#include <math.h>

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