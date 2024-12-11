#include "../include/Transform.h"

Transform::Transform()
{
    scale_axis = glm::vec3(1.0f, 1.0f, 1.0f);
    rotation_axis = glm::vec3(0.0f, 0.0f, 0.0f);
    translation = glm::vec3(0.0f, 0.0f, 0.0f);
}