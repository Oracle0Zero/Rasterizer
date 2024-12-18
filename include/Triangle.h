#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "../include/Point.h"

class Triangle
{
public:
    sf::Color color;
    std::vector<Point> vertices;
    bool null = false;
    Triangle(std::vector<Point> vertices_arg, sf::Color color_arg, bool null_arg = false);
};