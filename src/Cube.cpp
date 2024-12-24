#include "../include/Cube.h"

Cube::Cube()
{
    vertices.push_back(Point{1, 1, 1});
    vertices.push_back(Point{-1, 1, 1});
    vertices.push_back(Point{-1, -1, 1});
    vertices.push_back(Point{1, -1, 1});
    vertices.push_back(Point{1, 1, -1});
    vertices.push_back(Point{-1, 1, -1});
    vertices.push_back(Point{-1, -1, -1});
    vertices.push_back(Point{1, -1, -1});

    std::vector<int> indices = {0, 1, 2};
    triangles.push_back(Triangle{std::vector<Point>{vertices[0], \
    vertices[1], vertices[2]}, sf::Color::Red});

    indices = {0, 2, 3};
    triangles.push_back(Triangle{std::vector<Point>{vertices[0], \
    vertices[2], vertices[3]}, sf::Color::Green});

    indices = {4, 0, 3};
    triangles.push_back(Triangle{std::vector<Point>{vertices[4], \
    vertices[0], vertices[3]}, sf::Color::Black});

    indices = {4, 3, 7};
    triangles.push_back(Triangle{std::vector<Point>{vertices[4], \
    vertices[3], vertices[7]}, sf::Color::Blue}); 

    indices = {5, 4, 7};
    triangles.push_back(Triangle{std::vector<Point>{vertices[5], \
    vertices[4], vertices[7]}, sf::Color::Cyan});

    indices = {5, 7, 6};
    triangles.push_back(Triangle{std::vector<Point>{vertices[5], \
    vertices[7], vertices[6]}, sf::Color::Magenta}); 

    indices = {1, 5, 6};
    triangles.push_back(Triangle{std::vector<Point>{vertices[1], \
    vertices[5], vertices[6]}, sf::Color::Yellow});

    indices = {1, 6, 2};
    triangles.push_back(Triangle{std::vector<Point>{vertices[1], \
    vertices[6], vertices[2]}, sf::Color::Blue});

    indices = {4, 5, 1};
    triangles.push_back(Triangle{std::vector<Point>{vertices[4], \
    vertices[5], vertices[1]}, sf::Color::Green}); 

    indices = {4, 1, 0};
    triangles.push_back(Triangle{std::vector<Point>{vertices[4], \
    vertices[1], vertices[0]}, sf::Color::Blue}); 

    indices = {2, 6, 7};
    triangles.push_back(Triangle{std::vector<Point>{vertices[2], \
    vertices[6], vertices[7]}, sf::Color::Yellow}); 

    indices = {2, 7, 3};
    triangles.push_back(Triangle{std::vector<Point>{vertices[2], \
    vertices[7], vertices[3]}, sf::Color::Cyan});
}
    

void Cube::updateTriangles(std::vector<Point> projected)
{

    
        //std::vector<int> indices = {0, 1, 1};
        triangles[0].vertices[0] = projected[0];
        triangles[0].vertices[1] = projected[1];
        triangles[0].vertices[2] = projected[1];

        //indices = {0, 2, 3};
        triangles[1].vertices[0] = projected[0];
        triangles[1].vertices[1] = projected[2];
        triangles[1].vertices[2] = projected[3];

        //indices = {4, 0, 3};
        triangles[2].vertices[0] = projected[4];
        triangles[2].vertices[1] = projected[0];
        triangles[2].vertices[2] = projected[3];

        //indices = {4, 3, 7};
        triangles[3].vertices[0] = projected[4];
        triangles[3].vertices[1] = projected[3];
        triangles[3].vertices[2] = projected[7];

        //indices = {5, 4, 7};
        triangles[4].vertices[0] = projected[5];
        triangles[4].vertices[1] = projected[4];
        triangles[4].vertices[2] = projected[7];

        //indices = {5, 7, 6};
        triangles[5].vertices[0] = projected[5];
        triangles[5].vertices[1] = projected[7];
        triangles[5].vertices[2] = projected[6];

        //indices = {1, 5, 6};
        triangles[6].vertices[0] = projected[1];
        triangles[6].vertices[1] = projected[5];
        triangles[6].vertices[2] = projected[6];

        //indices = {1, 6, 2};
        triangles[7].vertices[0] = projected[1];
        triangles[7].vertices[1] = projected[6];
        triangles[7].vertices[2] = projected[2];

        //indices = {4, 5, 1};
        triangles[8].vertices[0] = projected[4];
        triangles[8].vertices[1] = projected[5];
        triangles[8].vertices[2] = projected[1];

        //indices = {4, 1, 0};
        triangles[9].vertices[0] = projected[4];
        triangles[9].vertices[1] = projected[1];
        triangles[9].vertices[2] = projected[0];

        //indices = {2, 6, 7};projected
        triangles[10].vertices[0] = projected[2];
        triangles[10].vertices[1] = projected[6];
        triangles[10].vertices[2] = projected[7];

        //indices = {2, 7, 3};
        triangles[11].vertices[0] = projected[2];
        triangles[11].vertices[1] = projected[7];
        triangles[11].vertices[2] = projected[3];
}