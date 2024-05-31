#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "embree.h"


class ray_tracer {
public:
    static int samples;

public:
    ray_tracer() = default;

    ~ray_tracer() = default;

    virtual float intersect_depth(const glm::vec3 &org, const glm::vec3 &dir) = 0;

    void raycasting(float *buffer, const glm::ivec2 &window_size, const Camera &cam);

    glm::vec3 ray_view_dir(const glm::ivec2 &pos, const glm::ivec2 &window_size, const glm::mat4 &inv_proj_view,
                           const glm::vec3 &cam_pos);
};


// TODO: this ray tracer implementation compute the ray triangle intersection with all the triangles in the scene.
class rt_simple : public ray_tracer {
    // TODO: add possible data members and class methods.

private:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal;
    };

    struct Triangle {
        Vertex v0;
        Vertex v1;
        Vertex v2;
    };

    std::vector<Triangle> triangles;
    bool in_shadow = false;
    glm::vec3 light_source;

public:
    // TODO: complete the definition of this method.
    float intersect_depth(const glm::vec3 &org, const glm::vec3 &dir) override
    {
        float min_distance = std::numeric_limits<float>::max();
        bool hit = false;
        in_shadow = false;

        for (const auto &triangle: triangles) {
            float distance;
            if (intersect_triangle(triangle, org, dir, distance)) {
                hit = true;
                if (distance < min_distance)
                    min_distance = distance;
                // launch shadow ray
                glm::vec3 shadow_dir = glm::normalize(light_source - (org + distance * dir));
                float shadow_distance;
                // if shadow ray intersects any object before reaching light source, point is in shadow
                in_shadow = intersect_depth(org + distance * dir, shadow_dir) > 0;
            }
        }

        return hit ? min_distance : 0.f;
    }

    bool is_in_shadow() const {
        return in_shadow;
    }
    bool shadow_ray_intersects(const glm::vec3 &org, const glm::vec3 &dir) {
        for (const auto &triangle: triangles) {
            float distance;
            if (intersect_triangle(triangle, org, dir, distance)) {
                return true;
            }
        }
        return false;
    }
    bool intersect_triangle(const Triangle &tri, const glm::vec3 &org, const glm::vec3 &dir, float & distance);
};


class rt_embree : public ray_tracer, public embree {
public:
    rt_embree(const std::vector<Shape *> &shapes) {
        for (auto &mesh: shapes)
            add_mesh(*mesh);

        build_bvh();
    }

    float intersect_depth(const glm::vec3 &org, const glm::vec3 &dir) {
        ray_hit r(org, dir);
        return intersect(r) ? r.ray.tfar : 0.f;
    }
};


#endif // RAY_TRACER_H

