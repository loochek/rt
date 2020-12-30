#ifndef MATH_LIB_H
#define MATH_LIB_H

#include <stdbool.h>

typedef struct vec3
{
    float x;
    float y;
    float z;
} vec3_t;

/**
 * Adds two vectors
 */
vec3_t vec_add(vec3_t a, vec3_t b);

/**
 * Substracts one vector from another
 */
vec3_t vec_sub(vec3_t from, vec3_t what);

/**
 * Multiplies vectors by-component
 */
vec3_t vec_mul(vec3_t a, vec3_t b);

/**
 * Multiplies vector and number
 */
vec3_t vec_mul_num(vec3_t vec, float num);

/**
 * Normalizes vector
 */
vec3_t vec_norm(vec3_t src_vec);

/**
 * Returns length of the vector
 */
float vec_length(vec3_t src_vec);

/**
 * Reflects vector with surface normal
 */
vec3_t vec_reflect(vec3_t src_vec, vec3_t norm);

/**
 * Calculates scalar product
 */
float vec_product(vec3_t a, vec3_t b);

bool less      (float a, float b);
bool more      (float a, float b);
bool less_or_eq(float a, float b);
bool more_or_eq(float a, float b);
bool equal     (float a, float b);

struct sphere;

/**
 * Checks intersection of ray and sphere
 * Gives intersection point if intersects
 */
bool ray_sphere_intersect(vec3_t *out_intersect_point,vec3_t ray_origin, vec3_t ray_dir,
                          struct sphere sphere);

#endif