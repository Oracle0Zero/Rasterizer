#pragma once

#include <glm/glm.hpp>
#include "Model.h"
#include "Transform.h"

class Instance
{
public:
    Model model;
    glm::vec3 translation_vector;
    Transform transform;
    glm::vec3 bounding_sphere_center;
    float bounding_shpere_radius;
};
