#pragma once

#include <glm/glm.hpp>

class Plane
{
private:
    glm::vec3 m_normal;
    float m_D;

public:
    Plane(glm::vec3 normal, float D);

    glm::vec3 GetNormal();
    float GetDistanceFromOrigin();

};