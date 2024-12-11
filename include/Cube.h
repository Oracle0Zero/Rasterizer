#pragma once

#include <vector>
#include "../include/Point.h"
#include "../include/Triangle.h"

class Cube
{
    // Cube Centered as (0, 0, 0)
public:
    std::vector<Point> vertices;
    std::vector<Triangle> triangles;
    float bounding_shpere_radius;
    Point bounding_sphere_center{0.0f, 0.0f, 0.0f};
    Cube();
    void updateTriangles(std::vector<Point> projected);
};