#include "../include/Instance.h"



void Instance::InitialBoundingSphere()
{
    Point center{0.0f, 0.0f, 0.0f};

    for(auto& p : cube.vertices)
    {
            
        center = center + p;
            
    }

    bounding_sphere_center.x = center.x / cube.vertices.size();
    bounding_sphere_center.y = center.y / cube.vertices.size();
    bounding_sphere_center.z = center.z / cube.vertices.size();
    //bounding_sphere_center.h = center.h / cube.vertices.size();

    bounding_shpere_radius = (cube.vertices[0] - bounding_sphere_center).Magnitude();
}