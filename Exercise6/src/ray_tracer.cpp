#include "ray_tracer.h"
#define EPSILON 0.00000001f

// TODO: play with the samples number, add a input control in the UI (user interface, ImGui window)
int ray_tracer::samples = 4;


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
bool rt_simple::intersect_triangle(const rt_simple::Triangle &tri, const glm::vec3 &org, const glm::vec3 &dir,
                                    float &distance) {
    glm::vec3 e1 = tri.v1.position - tri.v0.position;
    glm::vec3 e2 = tri.v2.position - tri.v0.position;

    glm::vec3 h = glm::cross(dir, e2);
    float a = glm::dot(e1, h);

    if(a > -EPSILON && a < EPSILON)
        return false; // ray paral

}

