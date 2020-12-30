#include <math.h>

#include "rt.h"

const float INF = 1e9;

color_t ray_trace(vec3_t ray_origin, vec3_t ray_dir, scene_t scene)
{
    sphere_t nearest_sphere = {0};
    float min_dist = INF;

    vec3_t intersect_point = {0};

    // find nearest sphere which intersects with ray

    for (size_t i = 0; i < scene.spheres_count; i++)
    {
        if (ray_sphere_intersect(&intersect_point, ray_origin, ray_dir, scene.spheres[i]))
        {
            float dist = vec_length(vec_sub(intersect_point, ray_origin));
            if (dist < min_dist)
            {
                min_dist = dist;
                nearest_sphere = scene.spheres[i];
            }
        }
    }

    if (equal(min_dist, INF))
        return scene.ambient_color;

    color_t result_color = {0};

    // calculate pixel color

    for (size_t i = 0; i < scene.lights_count; i++)
    {
        vec3_t light_vec   = vec_norm(vec_sub(scene.lights[i].position, intersect_point));
        vec3_t sphere_norm = vec_norm(vec_sub(intersect_point, nearest_sphere.position));

        // check for shadow

        bool shadowed = false;
        for (size_t j = 0; j < scene.spheres_count; j++)
        {
            // slightly move test point along normal to ignore testing surface
            vec3_t test_point = vec_add(intersect_point, vec_mul_num(sphere_norm, 1e-1));
            
            vec3_t dummy;
            if (ray_sphere_intersect(&dummy, test_point, light_vec, scene.spheres[j]))
            {
                shadowed = true;
                break; 
            }
        }
        
        float  diffuse_intensity = vec_product(sphere_norm, light_vec);

        vec3_t view_vec           = vec_norm(vec_sub(intersect_point, scene.camera_pos));
        vec3_t reflect_vec        = vec_reflect(vec_mul_num(light_vec, -1.f), sphere_norm);
        float  specular_intensity = vec_product(vec_mul_num(view_vec , -1.f), reflect_vec);

        if (diffuse_intensity < 0)
            diffuse_intensity = 0;

        if (specular_intensity < 0)
            specular_intensity = 0;

        specular_intensity = powf(specular_intensity, nearest_sphere.material.shininess);

        color_t ambient  = vec_mul(scene.lights[i].ambient, nearest_sphere.material.ambient);
        color_t diffuse  = vec_mul(scene.lights[i].diffuse,
                                vec_mul_num(nearest_sphere.material.diffuse, diffuse_intensity));
        color_t specular = vec_mul(scene.lights[i].specular,
                                vec_mul_num(nearest_sphere.material.specular, specular_intensity));

        if (shadowed)
            result_color = vec_add(result_color, ambient);
        else
            result_color = vec_add(result_color, vec_add(vec_add(ambient, diffuse), specular));
    }

    return result_color;
}