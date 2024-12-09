#include "Plane.h"

Plane::Plane(glm::vec3 normal, float D) : m_normal {normal}, m_D {D}
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