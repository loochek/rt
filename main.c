#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "math_lib.h"
#include "rt.h"

int main()
{
    size_t raster_rect_width = 3840, raster_rect_height = 2160;

    unsigned char *bitmap = calloc(raster_rect_width * raster_rect_height * 3, sizeof(unsigned char));

    // frustum config

    vec3_t raster_rect_vert1 = { -8.f, -4.5f, 0.f };
    vec3_t raster_rect_vert2 = {  8.f,  4.5f, 0.f };
    vec3_t camera_origin     = {  0.f,  0.f, -4.f };

    // sphere

    sphere_t spheres[2] = {};
    spheres[0].position = (vec3_t){ -10.f, 0.f, 10.f };
    spheres[0].radius   = 8.f;
    spheres[0].material.ambient   = (vec3_t){ 1.0f, 0.5f, 0.31f };
    spheres[0].material.diffuse   = (vec3_t){ 1.0f, 0.5f, 0.31f };
    spheres[0].material.specular  = (vec3_t){ 0.5f, 0.5f, 0.5f  };
    spheres[0].material.shininess = 32.f;

    spheres[1].position = (vec3_t){ 10.f, 0.f, 10.f };
    spheres[1].radius   = 8.f;
    spheres[1].material.ambient   = (vec3_t){ 1.0f, 0.5f, 0.31f };
    spheres[1].material.diffuse   = (vec3_t){ 1.0f, 0.5f, 0.31f };
    spheres[1].material.specular  = (vec3_t){ 0.5f, 0.5f, 0.5f  };
    spheres[1].material.shininess = 32.f;

    // light

    light_t light  = {0};
    light.position = (vec3_t){ 5.f, -5.f, 0.f };

    light.ambient   = (vec3_t){ 0.0f, 0.0f, 0.0f };
    light.diffuse   = (vec3_t){ 0.5f, 0.5f, 0.5f };
    light.specular  = (vec3_t){ 1.0f, 1.0f, 1.0f };

    scene_t scene = {0};
    scene.spheres = spheres;
    scene.spheres_count = 2;
    scene.lights = &light;
    scene.ambient_color = (vec3_t){ 0.1f, 0.1f, 0.1f };

    for (size_t y = 0; y < raster_rect_height; y++)
    {
        for (size_t x = 0; x < raster_rect_width; x++)
        {
            vec3_t raster_rect_dir = vec_sub(raster_rect_vert2, raster_rect_vert1);

            float pixel_width  = raster_rect_dir.x / raster_rect_width;
            float pixel_height = raster_rect_dir.y / raster_rect_height;

            vec3_t pixel_pos = { raster_rect_vert1.x + pixel_width  * ((float)x + 0.5f),
                                 raster_rect_vert1.y + pixel_height * ((float)y + 0.5f),
                                 -1.f };

            vec3_t ray_dir = vec_sub(pixel_pos, camera_origin);

            color_t color = ray_trace(camera_origin, ray_dir, scene);

            if (255 * color.x > 255.f)
                bitmap[y * 3 * raster_rect_width + 3 * x] = 255;
            else
                bitmap[y * 3 * raster_rect_width + 3 * x] = 255 * color.x;
            
            if (255 * color.y > 255.f)
                bitmap[y * 3 * raster_rect_width + 3 * x + 1] = 255;
            else
                bitmap[y * 3 * raster_rect_width + 3 * x + 1] = 255 * color.y;

            if (255 * color.z > 255.f)
                bitmap[y * 3 * raster_rect_width + 3 * x + 2] = 255;
            else
                bitmap[y * 3 * raster_rect_width + 3 * x + 2] = 255 * color.z;
        }
    }

    int status = stbi_write_png("test.png", raster_rect_width, raster_rect_height, 3, bitmap, 0);

    free(bitmap);
    return 0;
}