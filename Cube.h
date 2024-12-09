#pragma once

#include <vector>
#include "Point.h"
#include "Triangle.h"

class Cube
{
    // Cube Centered as (0, 0, 0)
public:
    std::vector<Point> vertices;
    std::vector<Triangle> triangles;
    float bounding_shpere_radius;
    Point bounding_sphere_center{0.0f, 0.0f, 0.0f};

    Cube()
    {
        vertices.push_back(Point{1, 1, 1});
        vertices.push_back(Point{-1, 1, 1});
        vertices.push_back(Point{-1, -1, 1});
        vertices.push_back(Point{1, -1, 1});
        vertices.push_back(Point{1, 1, -1});
        vertices.push_back(Point{-1, 1, -1});
        vertices.push_back(Point{-1, -1, -1});
        vertices.push_back(Point{1, -1, -1});


        std::vector<int> indices = {0, 1, 1};
        triangles.push_back(Triangle{indices, sf::Color::Red});
        indices = {0, 2, 3};
        triangles.push_back(Triangle{indices, sf::Color::Red});
        indices = {4, 0, 3};
        triangles.push_back(Triangle{indices, sf::Color::Green});
        indices = {4, 3, 7};
        triangles.push_back(Triangle{indices, sf::Color::Green}); 
        indices = {5, 4, 7};
        triangles.push_back(Triangle{indices, sf::Color::Blue}); 
        indices = {5, 7, 6};
        triangles.push_back(Triangle{indices, sf::Color::Blue}); 
        indices = {1, 5, 6};
        triangles.push_back(Triangle{indices, sf::Color::Yellow}); 
        indices = {1, 6, 2};
        triangles.push_back(Triangle{indices, sf::Color::Yellow}); 
        indices = {4, 5, 1};
        triangles.push_back(Triangle{indices, sf::Color::Magenta}); 
        indices = {4, 1, 0};
        triangles.push_back(Triangle{indices, sf::Color::Magenta}); 
        indices = {2, 6, 7};
        triangles.push_back(Triangle{indices, sf::Color::Cyan}); 
        indices = {2, 7, 3};
        triangles.push_back(Triangle{indices, sf::Color::Cyan}); 
    }
    
};