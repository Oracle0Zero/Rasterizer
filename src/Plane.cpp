#include "../include/Plane.h"

Plane::Plane(glm::vec3 normal, float D, std::string name) : m_normal {normal}, \
m_D {D}, m_name {name}
{

}

glm::vec3 Plane::GetNormal()
{
    return m_normal;
}

float Plane::GetDistanceFromOrigin()
{
    return m_D;
}

std::string Plane::getName()
{
    return m_name;
}