#include "../include/Point.h"


Point::Point()
{
    x = 0.0; y = 0.0; z = 0.0; h = 1.0;
}

Point operator+(const Point& p1, const glm::vec3& p2)
{
    Point new_point{p1.x, p1.y, p1.z, p1.h};
    new_point.x = p1.x + p2.x;
    new_point.y = p1.y + p2.y;
    new_point.z = p1.z + p2.z;
    return new_point;
}




Point operator+(const Point& p1, const Point& p2)
{
    Point new_point{p1.x, p1.y, p1.z, p1.h};
    new_point.x = p1.x + p2.x;
    new_point.y = p1.y + p2.y;
    new_point.z = p1.z + p2.z;
    new_point.h = p1.h + p2.h;
    return new_point;
}

Point operator-(const Point& p1, const Point& p2)
{
    Point new_point{0.0, 0.0, 0.0, 1.0};

    new_point.x = p2.x - p1.x;
    new_point.y = p2.y - p1.y;
    new_point.z = p2.z - p1.z;
    new_point.h = p2.h - p1.h;
    
    return new_point;
}