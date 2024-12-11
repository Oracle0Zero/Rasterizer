#pragma once

#include <glm/glm.hpp>

class Point
{
public:
    float x;
    float y;
    float z;
    float h;
    Point();
    Point(float x_coord, float y_coord, float z_coord, float h_intensity=1.0f)
    {
        x = x_coord;
        y = y_coord;
        z = z_coord;
        h = h_intensity;
    }

    float Magnitude()
    {
        glm::vec3 v(x, y, z);

        return glm::length(v);
    }


};

Point operator+(const Point& p1, const glm::vec3& p2);
Point operator+(const Point& p1, const Point& p2);
Point operator-(const Point& p1, const Point& p2);
