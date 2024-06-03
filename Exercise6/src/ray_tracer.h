#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "embree.h"


class ray_tracer {

protected:
    std::vector<Shape *> rt_shapes;
public:
    static int samples;

public:
    ray_tracer() = default;

    ~ray_tracer() = default;

    virtual float intersect_depth(const glm::vec3 &org, const glm::vec3 &dir) = 0;
    virtual bool shadow_ray_intersects(const glm::vec3 & org, const glm::vec3 & dir) = 0;
    virtual glm::vec3 color_intersect(const glm::vec3 & org, const glm::vec3 & dir, const glm::vec3 & light) = 0;


    void raycasting(float *buffer, const glm::ivec2 &window_size, const Camera &cam);
    void render(glm::vec3 * img, const glm::ivec2 & window_size, const Camera & cam, const glm::vec3 & light);

    glm::vec3 ray_view_dir(const glm::ivec2 &pos, const glm::ivec2 &window_size, const glm::mat4 &inv_proj_view,
                           const glm::vec3 &cam_pos);

    glm::vec3 calculate_color(Shape *shape, const int & triangle_id, const float & u, const float & v,
                              const glm::vec3 & light, const glm::vec3 & camera_position);
};


// TODO: this ray tracer implementation compute the ray triangle intersection with all the triangles in the scene.
class rt_simple : public ray_tracer {
    // TODO: add possible data members and class methods.

private:
    struct IntersectObject {
        int shape_id = -1;
        int triangle_id = -1;
        float u = 0.0f;
        float v = 0.0f;
        float depth = 1e8;
    };

public:
    explicit rt_simple(const std::vector<Shape *> &shapes) {
        rt_shapes = shapes;
    }

    // TODO: complete the definition of this method.
    float intersect_depth(const glm::vec3 &org, const glm::vec3 &dir) override {
        float min_distance = std::numeric_limits<float>::max();
        IntersectObject intersect = intersect_shape(org, dir);

        return intersect.depth > 0.00f ? min_distance : 0.f;
    }

    bool shadow_ray_intersects(const glm::vec3 &org, const glm::vec3 &dir) override {
        IntersectObject intersect = intersect_shape(org, dir);
        return intersect.depth > 0.00f;
    }

    IntersectObject intersect_shape(const glm::vec3 &org, const glm::vec3 &dir) {
        IntersectObject intersect;
        for (int i = 0; i < rt_shapes.size(); i++) {
            const Shape *shape_pointer = rt_shapes[i];
            for (int j = 0; j < shape_pointer->faces.size(); j++) {
                const glm::uvec3 &triangle = shape_pointer->faces[j];
                const glm::mat4 modelMatrix = shape_pointer->getModelMatrix();

                // get vertices of the triangle
                glm::vec3 vertex0 = glm::vec3(modelMatrix * glm::vec4(shape_pointer->positions[triangle[0]], 1.0f));
                glm::vec3 vertex1 = glm::vec3(modelMatrix * glm::vec4(shape_pointer->positions[triangle[1]], 1.0f));
                glm::vec3 vertex2 = glm::vec3(modelMatrix * glm::vec4(shape_pointer->positions[triangle[2]], 1.0f));

                // compute vectors along the edges
                glm::vec3 edge1 = vertex1 - vertex0;
                glm::vec3 edge2 = vertex2 - vertex0;

                // compute the determinant
                glm::vec3 p_vector = glm::cross(dir, edge2);
                float determinant = glm::dot(edge1, p_vector);

                // if the determinant is negative, the triangle is backfacing
                if (determinant > -1e-8 && determinant < 1e-8)
                    continue;
                float determinant_inverse = 1.0f / determinant;

                // calculate distance from vertex0 to the ray origin
                glm::vec3 t_vector = org - vertex0;

                // calculate u parameter and test bound
                float u = glm::dot(t_vector, p_vector) * determinant_inverse;
                if (u < 0.0f || u > 1.0f)
                    continue;

                // calculate v parameter and test bound
                glm::vec3 q_vector = glm::cross(t_vector, edge1);
                float v = glm::dot(dir, q_vector) * determinant_inverse;
                if (v < 0.0f || u + v > 1.0f)
                    continue;

                // calculate t
                float depth = glm::dot(edge2, q_vector) * determinant_inverse;

                if (depth < intersect.depth) {
                    intersect.depth = depth;
                    intersect.shape_id = i;
                    intersect.triangle_id = j;
                }

            }
        }
        return intersect;
    }

    glm::vec3 color_intersect(const glm::vec3 & org, const glm::vec3 & dir, const glm::vec3 & light) {
        const IntersectObject & iO = intersect_shape(org, dir);
        if(iO.depth < 1e8) return {};

        return calculate_color(rt_shapes[iO.shape_id], iO.triangle_id, iO.u, iO.v, light, org);
    }
};


class rt_embree : public ray_tracer, public embree {

public:
    rt_embree(const std::vector<Shape *> &shapes) {
        for (auto &mesh: shapes) {
            add_mesh(*mesh);
            rt_shapes.push_back(mesh);
        }

        build_bvh();
    }

    float intersect_depth(const glm::vec3 &org, const glm::vec3 &dir) override {
        ray_hit r(org, dir);
        return intersect(r) ? r.ray.tfar : 0.f;
    }

    bool shadow_ray_intersects(const glm::vec3 &org, const glm::vec3 &dir) override {
        ray_hit r(org, dir);
        return intersect(r);
    }

    glm::vec3 color_intersect(const glm::vec3 & org, const glm::vec3 & dir, const glm::vec3 & light) {
        ray_hit r(org, dir);
        if(!intersect(r)) return {};

        return calculate_color(rt_shapes[r.hit.geomID], r.hit.primID, r.hit.u, r.hit.v, light, org);
    }
};


#endif // RAY_TRACER_H

