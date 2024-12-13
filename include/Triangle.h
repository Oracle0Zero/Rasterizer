#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "../include/Point.h"

class Triangle
{
public:
    std::vector<int> point_indices;
    std::vector<Point> vertices;
    sf::Color color;
    bool null = false;
};