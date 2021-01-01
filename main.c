#include <stdlib.h>
#include <time.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "math_lib.h"
#include "rt.h"

static void render(scene_t scene, size_t frame_cnt)
{
    size_t raster_rect_width = 3840, raster_rect_height = 2160;

    unsigned char *bitmap = calloc(raster_rect_width * raster_rect_height * 3, sizeof(unsigned char));

    // camera config

    vec3_t raster_rect_vert1 = { -8.f, -4.5f, 0.f };
    vec3_t raster_rect_vert2 = {  8.f,  4.5f, 0.f };
    vec3_t camera_origin     = {  0.f,  0.f, -24.f };

    for (size_t y = 0; y < raster_rect_height; y++)
    {
        for (size_t x = 0; x < raster_rect_width; x++)
        {
            vec3_t raster_rect_dir = vec_sub(raster_rect_vert2, raster_rect_vert1);

            float pixel_width  = raster_rect_dir.x / raster_rect_width;
            float pixel_height = raster_rect_dir.y / raster_rect_height;

            vec3_t pixel_pos = { raster_rect_vert1.x + pixel_width  * ((float)x + 0.5f),
                                 raster_rect_vert1.y + pixel_height * ((float)y + 0.5f),
                                 -7.f };

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

    char file_name[16];
    snprintf(file_name, 15, "test%zu.png", frame_cnt);

    stbi_write_png(file_name, raster_rect_width, raster_rect_height, 3, bitmap, 0);

    free(bitmap);
}

int main()
{

    // materials

    material_t matte = { 0 };

    matte.ambient   = (vec3_t){ 1.0f, 0.5f, 0.31f };
    matte.diffuse   = (vec3_t){ 1.0f, 0.5f, 0.31f };
    matte.specular  = (vec3_t){ 0.2f, 0.2f, 0.2f  };
    matte.shininess = 4.f;

    material_t gloss = { 0 };

    gloss.ambient   = (vec3_t){ 0.8f, 0.9f, 0.2f };
    gloss.diffuse   = (vec3_t){ 0.8f, 0.9f, 0.2f };
    gloss.specular  = (vec3_t){ 0.9f, 0.9f, 0.9f  };
    gloss.shininess = 32.f;

    material_t mate_blue = { 0 };

    mate_blue.ambient   = (vec3_t){ 0.2f, 0.2f, 0.7f };
    mate_blue.diffuse   = (vec3_t){ 0.2f, 0.2f, 0.7f };
    mate_blue.specular  = (vec3_t){ 0.1f, 0.1f, 0.1f  };
    mate_blue.shininess = 64.f;

    // spheres

    sphere_t spheres[2] = {};
    spheres[0].position = (vec3_t){  7.f, -1.f, 10.f };
    spheres[0].radius   = 6.f;
    spheres[0].material = matte;

    spheres[1].position = (vec3_t){ -7.f, -1.f, 10.f };
    spheres[1].radius   = 6.f;
    spheres[1].material = gloss;

    // plane

    plane_t plane;
    plane.position = (vec3_t){ 0.f,  7.f, 15.f };
    plane.norm     = (vec3_t){ 0.f, -1.f,  0.f };
    plane.radius   = 20.f;
    plane.material = mate_blue;

    // light

    light_t lights[2]  = {0};
    lights[0].position = (vec3_t){ -17.f, -10.f, -12.f };
    lights[1].position = (vec3_t){  17.f, -10.f, -12.f };

    lights[0].ambient   = (vec3_t){ 0.2f, 0.2f, 0.2f };
    lights[0].diffuse   = (vec3_t){ 0.5f, 0.5f, 0.5f };
    lights[0].specular  = (vec3_t){ 1.0f, 1.0f, 1.0f };

    lights[1].ambient   = (vec3_t){ 0.2f, 0.2f, 0.2f };
    lights[1].diffuse   = (vec3_t){ 0.5f, 0.5f, 0.5f };
    lights[1].specular  = (vec3_t){ 1.0f, 1.0f, 1.0f };

    scene_t scene = {0};
    scene.ambient_color = (vec3_t){ 0.1f, 0.1f, 0.1f };

    scene.spheres       = spheres;
    scene.spheres_count = 2;

    scene.planes        = &plane;
    scene.planes_count  = 1;

    scene.lights        = lights;
    scene.lights_count  = 2;

    render(scene, 0);
    return 0;
}