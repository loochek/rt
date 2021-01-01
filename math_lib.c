#include <math.h>

#include "math_lib.h"
#include "rt.h"

const float EPS = 1e-5;

vec3_t vec_add(vec3_t a, vec3_t b)
{
    return (vec3_t){ a.x + b.x,
                     a.y + b.y,
                     a.z + b.z };
}

vec3_t vec_sub(vec3_t from, vec3_t what)
{
    return (vec3_t){ from.x - what.x,
                     from.y - what.y,
                     from.z - what.z };
}

vec3_t vec_norm(vec3_t src_vec)
{
    float length = vec_length(src_vec);

    return (vec3_t){ src_vec.x / length,
                     src_vec.y / length,
                     src_vec.z / length };
}

vec3_t vec_mul(vec3_t a, vec3_t b)
{
    return (vec3_t){ a.x * b.x,
                     a.y * b.y,
                     a.z * b.z };
}

float vec_length(vec3_t src_vec)
{
    return sqrtf(vec_product(src_vec, src_vec));
}

vec3_t vec_reflect(vec3_t src_vec, vec3_t norm)
{
    return vec_add(src_vec, vec_mul_num(norm, -2.f * vec_product(src_vec, norm)));
}

vec3_t vec_mul_num(vec3_t vec, float num)
{
    return (vec3_t){ vec.x * num,
                     vec.y * num,
                     vec.z * num };
}

float vec_product(vec3_t a, vec3_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool less(float a, float b)
{
    return a - b < -EPS;
}

bool more(float a, float b)
{
    return a - b > EPS;
}

bool less_or_eq(float a, float b)
{
    return a - b < EPS;
}

bool more_or_eq(float a, float b)
{
    return a - b > -EPS;
}

bool equal(float a, float b)
{
    return fabsf(a - b) <= EPS;
}

/**
 * The check is done by solving:
 * {
 *     x = A + d * t, // parameterized equation of ray, t > 0
 *     |x - B|^2 = r^2 // sphere equation
 * }, where A, B are radius-vectors of ray origin and sphere center, r is the sphere radius
 * |A - B + d * t|^2 = r^2
 * s = A - B, so
 * (s + d * t)^2 = r^2
 * ...
 * so gets quadratic equation with unknown parameter
 * least positive solution is parameter of intersection point
 */
bool ray_sphere_intersect(vec3_t *out_intersect_point,vec3_t ray_origin, vec3_t ray_dir,
                          sphere_t sphere)
{
    ray_dir = vec_norm(ray_dir);

    vec3_t s = vec_sub(ray_origin, sphere.position);

    float a = vec_product(ray_dir, ray_dir);
    float b = 2 * vec_product(s, ray_dir);
    float c = vec_product(s, s) - sphere.radius * sphere.radius;

    float d = b * b - 4 * a * c;

    float t = 0;
    
    if (equal(d, 0))
    {
        float t1 = -b / (2 * a);

        if (t1 > EPS)
            t = t1;
    }
    else if (more(d, 0))
    {
        float t1 = (-b - sqrtf(d)) / (2 * a);
        float t2 = (-b + sqrtf(d)) / (2 * a);

        if (t1 > EPS)
            t = t1;
        else if (t2 > EPS)
            t = t2;
    }

    if (equal(t, 0))
        return false;

    *out_intersect_point = vec_add(ray_origin, vec_mul_num(ray_dir, t));
    return true;
}

bool ray_plane_intersect(vec3_t *out_intersect_point,vec3_t ray_origin, vec3_t ray_dir,
                         plane_t plane)
{
    ray_dir = vec_norm(ray_dir);

    float num   = vec_product(vec_sub(plane.position, ray_origin), plane.norm);
    float denom = vec_product(ray_dir, plane.norm);

    float param = num / denom;

    if (less_or_eq(param, 0))
        return false;

    *out_intersect_point = vec_add(ray_origin, vec_mul_num(ray_dir, param));

    if (more(vec_length(vec_sub(*out_intersect_point, plane.position)), plane.radius))
        return false;

    return true;
}