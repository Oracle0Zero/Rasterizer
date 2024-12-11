#pragma once

#include<glm/glm.hpp>
#include "../include/Transform.h"

class Camera
{
public:
    Transform camera_transform;
    Camera();
    void SetCameraPosition(glm::vec3 position);
    void SetCameraRotation(glm::vec3 rotation);
};