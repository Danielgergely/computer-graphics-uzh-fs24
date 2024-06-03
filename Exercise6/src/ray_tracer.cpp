#include "ray_tracer.h"

#define EPSILON 0.00000001f

// TODO: play with the samples number, add a input control in the UI (user interface, ImGui window)
int ray_tracer::samples = 2;


void ray_tracer::raycasting(float *buffer, const glm::ivec2 &window_size, const Camera &cam) {
    glm::mat4 inv_proj_view = glm::inverse(cam.getViewProjectionMatrix());

#pragma omp parallel for
    for (int i = 0; i < window_size.x; ++i)
        for (int j = 0; j < window_size.y; ++j) {
            //row major
            float &depth = buffer[(window_size.y - j - 1) * window_size.x + i] = 0;
            glm::vec3 dir = ray_view_dir({i, j}, window_size, inv_proj_view, cam.getPosition());

            for (int s = 0; s < samples; ++s)
                depth += intersect_depth(cam.getPosition(), dir);

            depth /= samples;
        }
}


glm::vec3 ray_tracer::ray_view_dir(const glm::ivec2 &pos, const glm::ivec2 &window_size, const glm::mat4 &inv_proj_view,
                                   const glm::vec3 &cam_pos) {
    // TODO: implement this function to compute the direction of primary rays with a random offset.
    // HINT: do this first, and test it with the embree tracer, then complete the rt_simple class.

    // Get the normalized device coordinates
    glm::vec2 ndc = glm::vec2(pos) / glm::vec2(window_size) * 2.0f - 1.0f;

    // Apply random offset -> anti-aliasing
    //ndc += glm::vec2((float)rand() / RAND_MAX - 0.5f, (float)rand() / RAND_MAX - 0.5f) / glm::vec2(window_size);

    // Get view space coordinates
    glm::vec4 view = inv_proj_view * glm::vec4(ndc.x, ndc.y, -1.0f, 1.0f);
    view /= view.w;

    // Get the direction
    glm::vec3 dir = glm::normalize(glm::vec3(view) - cam_pos);
    return dir;
}

// TODO: add the definition of the methods here.

void ray_tracer::render(glm::vec3 *img, const glm::ivec2 &window_size, const Camera &cam, const glm::vec3 &light) {
    glm::mat4 inv_proj_view = glm::inverse(cam.getViewProjectionMatrix());

    #pragma omp parallel for
    for (int i = 0; i < window_size.x; ++i)
        for (int j = 0; j < window_size.y; ++j) {
            //row major
            glm::vec3 & color = img[j * window_size.x + i];
            glm::vec3 dir = ray_view_dir({i, j}, window_size, inv_proj_view, cam.getPosition());

            color = glm::vec3(0);
            for(int s = 0; s < samples; ++s)
                color += color_intersect(cam.getPosition(), dir, light);

            color /= samples;
        }

}


glm::vec3 shading(const std::vector<glm::vec3> &values, const glm::uvec3 &triangle, const float &u, const float &v) {
    // Calculate the weighted average of the values at the vertices of the triangle
    glm::vec3 result = (1 - u - v) * values[triangle[0]] +
                       u * values[triangle[1]] +
                       v * values[triangle[2]];
    return result;
}

glm::vec3 ray_tracer::calculate_color(cgCourse::Shape *shape, const int &triangle_id, const float &u, const float &v,
                                      const glm::vec3 &light, const glm::vec3 &camera_position) {

    const glm::mat4 &model_matrix = shape->getModelMatrix();
    const glm::uvec3 &triangle = shape->faces[triangle_id];

    glm::vec3 pos = glm::vec3(model_matrix * glm::vec4(shading(shape->positions, triangle, u, v), 1.0f));
    glm::vec3 normal = glm::normalize(
            glm::vec3(model_matrix * glm::vec4(shading(shape->normals, triangle, u, v), 0.0f)));
    glm::vec3 color = shading(shape->colors, triangle, u, v);

    // Calculate the light direction
    glm::vec3 light_direction = glm::normalize(light - pos);

    float dot_difference = std::max(dot(normal, light_direction), 0.0f);
    glm::vec3 diffuseColor = dot_difference * glm::vec3(1);

    glm::vec3 view_direction = normalize(camera_position - pos);
    glm::vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_dot = std::max(dot(view_direction, reflect_direction), 0.0f);
    float spec = pow(specular_dot, 32);
    float specular_strength = 1.0;
    glm::vec3 specular_color = specular_strength * spec * glm::vec3(1);

    float s = shadow_ray_intersects(pos, light_direction) ? 0.4 : 1;

    const glm::vec3 ambient_color(0.25f);

    return ((ambient_color + diffuseColor) * color + specular_color * 0.5f) * s;


}

