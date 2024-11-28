#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <string>

constexpr int canvas_width = 800;
constexpr int canvas_height = 800;

constexpr int viewport_width = 1;
constexpr int viewport_height = 1;

class Point
{
public:
    float x;
    float y;
    float h;

    Point(float x_coord, float y_coord, float h_intensity=1.0f)
    {
        x = x_coord;
        y = y_coord;
    }
};

void PutPixel(sf::RenderWindow& window, sf::RectangleShape& pixel, int x, int y, sf::Color color);
glm::vec3 CanvasToViewPort(int x, int y);
void DrawLine(Point p0, Point p1, sf::Color color);
std::vector<float> Interpolate(float i0, float d0, float i1, float d1);
void DrawWireframeTriangle(Point p0, Point p1, Point p2, sf::Color color);
void SwapPoints(Point& p0, Point& p1);
void DrawFilledTriangle(Point p0, Point p1, Point p2, sf::Color color);

sf::RenderWindow window(sf::VideoMode(canvas_width, canvas_height), "SFML works!");
sf::RectangleShape pixel(sf::Vector2f(1, 1));
int main()
{

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        DrawFilledTriangle(Point{-200, -250}, Point{200, 50}, Point{20, 250}, sf::Color::Green);

        //DrawLine(glm::vec2(-200, -250), glm::vec2(200, 50), sf::Color::Black);
        DrawWireframeTriangle(Point{-200, -250}, Point{200, 50}, Point{20, 250}, sf::Color::Black);

        window.display();
    }

    return 0;
}

void PutPixel(sf::RenderWindow& window, sf::RectangleShape& pixel, int x, int y, sf::Color color)
{
    float c_x = (float)canvas_width / 2 + x;
    float c_y = (float)canvas_height / 2 - y;
    
    //sf::RectangleShape pixel(sf::Vector2f(1, 1));
    pixel.setPosition(c_x, c_y);
    pixel.setFillColor(color);

    window.draw(pixel);
}

glm::vec3 CanvasToViewPort(int x, int y)
{
    float v_x = x * ((float)viewport_width / (float)canvas_width);
    float v_y = y * ((float)viewport_height / (float)canvas_height);

    return glm::vec3(v_x, v_y, 1);
}

void DrawLine(Point p0, Point p1, sf::Color color)
{
    float dx = p1.x - p0.x;
    float dy = p1.y - p0.y;

    /*
    printf("p0.x: %f\n", p0.x);
    printf("p0.y: %f\n", p0.y);
    printf("p1.x: %f\n", p1.x);
    printf("p1.y: %f\n", p1.y);
    printf("dx: %f\n", dx);
    printf("dy: %f\n", dy);
    */

    if(abs(dx) > abs(dy))
    {
        // Line is horizontal-ish
        // Make sure p0.x < p1.x
        if(p0.x > p1.x)
        {
            SwapPoints(p0, p1);
        }  
        //float a = dy/dx;
        std::vector<float> ys = Interpolate(p0.x, p0.y, p1.x, p1.y);
        for(float x = p0.x; x <= p1.x; ++x)
        {
            PutPixel(window, pixel, static_cast<int>(x), static_cast<int>(ys[static_cast<int>(x - p0.x)]), color);
        }
    }else
    {
        // Line is vertical-ish
        // Make sure p0.y < p1.y
        if(p0.y > p1.y)
        {
            SwapPoints(p0, p1);
        }
        //float a = dx/dy;
        std::vector<float> xs = Interpolate(p0.y, p0.x, p1.y, p1.x);
        for(float y = p0.y; y <= p1.y; ++y)
        {
            PutPixel(window, pixel, static_cast<int>(xs[static_cast<int>(y - p0.y)]), static_cast<int>(y), color);
        }
    }
}

std::vector<float> Interpolate(float i0, float d0, float i1, float d1)
{
    std::vector<float> values;
    if(static_cast<int>(i0) == static_cast<int>(i1))
    {

        values.push_back(d0);
        return values;
    }
    float a = (d1 - d0) / (i1 - i0);
    float d = d0;
    for(float i = i0; i <= i1; i++)
    {
        values.push_back(d);
        d += a;
    }

    return values;
}

void DrawWireframeTriangle(Point p0, Point p1, Point p2, sf::Color color)
{
    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p0, color);
}

void SwapPoints(Point& p0, Point& p1)
{
    float temp;

    temp = p0.y;
    p0.y = p1.y;
    p1.y = temp;

    temp = p0.x;
    p0.x = p1.x;
    p1.x = temp;
}

void DrawFilledTriangle(Point p0, Point p1, Point p2, sf::Color color)
{
    // Sort the points so that y0 <= y1 <= y2
    if(p1.y < p0.y) { SwapPoints(p1, p0);}
    if(p2.y < p0.y) { SwapPoints(p2, p0);}
    if(p2.y < p1.y) { SwapPoints(p2, p1);}

    // Computer the x coordinates and h values of the triangle edges
    std::vector<float> x01 = Interpolate(p0.y, p0.x, p1.y, p1.x);
    std::vector<float> h01 = Interpolate(p0.y, p0.h, p1.y, p1.h);

    std::vector<float> x12 = Interpolate(p1.y, p1.x, p2.y, p2.x);
    std::vector<float> h12 = Interpolate(p1.y, p1.h, p2.y, p2.h);

    std::vector<float> x02 = Interpolate(p0.y, p0.x, p2.y, p2.x);

    
    // Concatenate short sides
    x01.pop_back();
    std::vector<float> x012;
    x012.insert(x012.end(), x01.begin(), x01.end());
    x012.insert(x012.end(), x12.begin(), x12.end());

    /*
    std::cout << "x01 size: " << x01.size() << "\n";
    std::cout << "x12 size: " << x12.size() << "\n";
    std::cout << "x02 size: " << x02.size() << "\n";
    std::cout << "x012 size: " << x012.size() << "\n";
    */

    std::vector<float> x_left;
    std::vector<float> x_right;

    // Determine which is left and which is right
    int m = static_cast<int>(floor(x012.size()/2));
    if(x02[m] < x012[m])
    {
        x_left = x02;
        x_right = x012;
    }else
    {
        x_left = x012;
        x_right = x02;
    }

    //std::cout << "x_left size: " << x_left.size() << "\n";
    //std::cout << "x_right size: " << x_right.size() << "\n";
    
    // Draw the horizontal segments
    for(float y = p0.y; y <= p2.y; y++)
    {
        for(float x = x_left[static_cast<int>(y - p0.y)]; x <= x_right[static_cast<int>(y - p0.y)]; x++)
        {
            PutPixel(window, pixel, static_cast<int>(x), static_cast<int>(y), color);
        }
    }

    
}
