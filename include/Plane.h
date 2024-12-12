#pragma once

#include <glm/glm.hpp>
#include <string>

class Plane
{
private:
    glm::vec3 m_normal;
    float m_D;
    std::string m_name;

public:
    Plane(glm::vec3 normal, float D, std::string name);
    glm::vec3 GetNormal();
    float GetDistanceFromOrigin();
    std::string getName();
};