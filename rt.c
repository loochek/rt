#include <math.h>

#include "rt.h"

const float INF = 1e9;

// TODO: plane has normal view only at "right side" of normal vector - fix it

/**
 * Kinda fragment shader:
 * common code to calculate color of fragment
 */
static color_t fragment_shader(vec3_t frag_pos, vec3_t norm, material_t mat, scene_t scene)
{
    norm = vec_norm(norm);

    color_t result_color = {0};

    // calculate pixel color

    for (size_t i = 0; i < scene.lights_count; i++)
    {
        vec3_t light_vec = vec_norm(vec_sub(scene.lights[i].position, frag_pos));

        // check for shadow

        bool shadowed = false;

        for (size_t j = 0; j < scene.spheres_count; j++)
        {
            // slightly move test point along normal to ignore testing surface
            vec3_t test_point = vec_add(frag_pos, vec_mul_num(norm, 1e-1));
            
            vec3_t intersect_point = {0};
            if (ray_sphere_intersect(&intersect_point, test_point, light_vec, scene.spheres[j]))
            {
                float light_dist     = vec_length(vec_sub(scene.lights[i].position, test_point));
                float intersect_dist = vec_length(vec_sub(intersect_point         , test_point));

                if (less_or_eq(intersect_dist, light_dist))
                {
                    shadowed = true;
                    break;
                }
            }
        }
        
        float  diffuse_intensity  = vec_product(norm, light_vec);

        vec3_t view_vec           = vec_norm(vec_sub(frag_pos, scene.camera_pos));
        vec3_t reflect_vec        = vec_norm(vec_reflect(vec_mul_num(light_vec, -1.f), norm));
        float  specular_intensity = vec_product(vec_mul_num(view_vec , -1.f), reflect_vec);

        if (diffuse_intensity < 0)
            diffuse_intensity = 0;

        if (specular_intensity < 0)
            specular_intensity = 0;

        specular_intensity = powf(specular_intensity, mat.shininess);

        color_t ambient  = vec_mul(scene.lights[i].ambient, mat.ambient);
        color_t diffuse  = vec_mul(scene.lights[i].diffuse,
                                   vec_mul_num(mat.diffuse, diffuse_intensity));
        color_t specular = vec_mul(scene.lights[i].specular,
                                   vec_mul_num(mat.specular, specular_intensity));

        if (shadowed)
            result_color = vec_add(result_color, ambient);
        else
            result_color = vec_add(result_color, vec_add(vec_add(ambient, diffuse), specular));
    }

    return result_color;
}

color_t ray_trace(vec3_t ray_origin, vec3_t ray_dir, scene_t scene)
{
    material_t nearest_frag_mat  = {0};
    vec3_t     nearest_frag_pos  = {0};
    vec3_t     nearest_frag_norm = {0};
    float      nearest_frag_dist = INF;

    // find nearest sphere or plane which intersects with ray

    for (size_t i = 0; i < scene.spheres_count; i++)
    {
        vec3_t intersect_point = {0};
        if (ray_sphere_intersect(&intersect_point, ray_origin, ray_dir, scene.spheres[i]))
        {
            float dist = vec_length(vec_sub(intersect_point, ray_origin));
            if (dist < nearest_frag_dist)
            {
                nearest_frag_dist  = dist;
                nearest_frag_mat   = scene.spheres[i].material;
                nearest_frag_pos   = intersect_point;
                nearest_frag_norm  = vec_norm(vec_sub(intersect_point, scene.spheres[i].position));
            }
        }
    }

    for (size_t i = 0; i < scene.planes_count; i++)
    {
        vec3_t intersect_point = {0};
        if (ray_plane_intersect(&intersect_point, ray_origin, ray_dir, scene.planes[i]))
        {
            float dist = vec_length(vec_sub(intersect_point, ray_origin));
            if (dist < nearest_frag_dist)
            {
                nearest_frag_dist  = dist;
                nearest_frag_mat   = scene.planes[i].material;
                nearest_frag_pos   = intersect_point;
                nearest_frag_norm  = scene.planes[i].norm;
            }
        }
    }

    if (equal(nearest_frag_dist, INF))
        return scene.ambient_color;

    return fragment_shader(nearest_frag_pos,
                           nearest_frag_norm,
                           nearest_frag_mat,
                           scene);
}