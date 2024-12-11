#pragma once

#include <glm/glm.hpp>
#include "../include/Transform.h"
#include "../include/Cube.h"

class Instance
{
public:
    Cube cube;
    glm::vec3 translation_vector;
    Transform transform;
    Point bounding_sphere_center;
    float bounding_sphere_radius;
    bool null = false;

    void InitialBoundingSphere();
};
