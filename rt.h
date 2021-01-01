#ifndef RT_H
#define RT_H

#include <stdlib.h>

#include "math_lib.h"

typedef vec3_t color_t;

typedef struct material
{
    color_t ambient;
    color_t diffuse;
    color_t specular;

    float   shininess;
} material_t;

typedef struct light
{
    vec3_t  position;

    color_t ambient;
    color_t diffuse;
    color_t specular;
} light_t;

typedef struct sphere
{
    vec3_t     position;
    float      radius;
    
    material_t material;
} sphere_t;

// plane is clipped by circle
typedef struct plane
{
    vec3_t     position;
    vec3_t     norm;
    float      radius;
    
    material_t material;
} plane_t;

typedef struct scene
{
    sphere_t *spheres;
    size_t    spheres_count;

    plane_t  *planes;
    size_t    planes_count;

    light_t  *lights;
    size_t    lights_count;

    color_t   ambient_color;

    vec3_t    camera_pos;
} scene_t;

color_t ray_trace(vec3_t ray_origin, vec3_t ray_dir, scene_t scene);

#endif