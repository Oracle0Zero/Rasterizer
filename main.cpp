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
    float z;
    float h;

    Point(float x_coord, float y_coord, float z_coord, float h_intensity=1.0f)
    {
        x = x_coord;
        y = y_coord;
        z = z_coord;
        h = h_intensity;
    }

  friend Point operator+(const Point& p1, const glm::vec3& p2);

};

Point operator+(const Point& p1, const glm::vec3& p2)
{
    Point new_point{p1.x, p1.y, p1.z, p1.h};
    new_point.x = p1.x + p2.x;
    new_point.y = p1.y + p2.y;
    new_point.z = p1.z + p2.z;
    return new_point;
}

class Triangle
{
public:
    std::vector<int> point_indices;
    sf::Color color;

};

class Model
{
    // Assume Mode is only made of one cube
public:
    std::vector<Cube> cubes;
};

class Instance
{
public:
    Model model;
    glm::vec3 translation_vector;
};

class Cube
{
    // Cube Centered as (0, 0, 0)
public:
    std::vector<Point> vertices;
    std::vector<Triangle> triangles;

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

void PutPixel(sf::RenderWindow& window, sf::RectangleShape& pixel, int x, int y, sf::Color color);
glm::vec3 CanvasToViewPort(int x, int y);
void DrawLine(Point p0, Point p1, sf::Color color);
std::vector<float> Interpolate(float i0, float d0, float i1, float d1);
void DrawWireframeTriangle(Point p0, Point p1, Point p2, sf::Color color);
void SwapPoints(Point& p0, Point& p1);
void DrawFilledTriangle(Point p0, Point p1, Point p2, sf::Color color);
sf::Color ModifiedColor(sf::Color color, float modifier);
glm::vec3 ViewportToCanvas(float x, float y, float d);
Point ProjectVertex(Point v);
void DrawCube();
void RenderTriangle(Triangle triangle, std::vector<Point>& projected);
void RenderObject(std::vector<Point> vertices, std::vector<Triangle> triangles);
void RenderScene(std::vector<Instance> instances);
void RenderInstance(Instance instance);

sf::RenderWindow window(sf::VideoMode(canvas_width, canvas_height), "Rasterizer");
sf::RectangleShape pixel(sf::Vector2f(1, 1));

float d = 1.0f;

int main()
{
    Cube c1;

    Model model;
    model.cubes.push_back(c1);

    Instance instance_1;
    instance_1.model = model;
    instance_1.translation_vector = glm::vec3(-1.5f, 0.0f, 7.0f);
    
    Instance instance_2;
    instance_2.model = model;
    instance_2.translation_vector = glm::vec3(1.25f, 2.0f, 7.5f);

    std::vector<Instance> instances;
    instances.push_back(instance_1);
    instances.push_back(instance_2);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);


        /*
        // The four "front" vertices
        Point vAf = Point{-2, -0.5, 5};
        Point vBf = Point{-2,  0.5, 5};
        Point vCf = Point{-1,  0.5, 5};
        Point vDf = Point{-1, -0.5, 5};

        // The four "back" vertices
        Point vAb = Point{-2, -0.5, 6};
        Point vBb = Point{-2,  0.5, 6};
        Point vCb = Point{-1,  0.5, 6};
        Point vDb = Point{-1, -0.5, 6};

        // The front face
        DrawLine(ProjectVertex(vAf), ProjectVertex(vBf), sf::Color::Blue);
        DrawLine(ProjectVertex(vBf), ProjectVertex(vCf), sf::Color::Blue);
        DrawLine(ProjectVertex(vCf), ProjectVertex(vDf), sf::Color::Blue);
        DrawLine(ProjectVertex(vDf), ProjectVertex(vAf), sf::Color::Blue);

        // The back face
        DrawLine(ProjectVertex(vAb), ProjectVertex(vBb), sf::Color::Red);
        DrawLine(ProjectVertex(vBb), ProjectVertex(vCb), sf::Color::Red);
        DrawLine(ProjectVertex(vCb), ProjectVertex(vDb), sf::Color::Red);
        DrawLine(ProjectVertex(vDb), ProjectVertex(vAb), sf::Color::Red);

        // The front-to-back edges
        DrawLine(ProjectVertex(vAf), ProjectVertex(vAb), sf::Color::Green);
        DrawLine(ProjectVertex(vBf), ProjectVertex(vBb), sf::Color::Green);
        DrawLine(ProjectVertex(vCf), ProjectVertex(vCb), sf::Color::Green);
        DrawLine(ProjectVertex(vDf), ProjectVertex(vDb), sf::Color::Green);

        */

        //DrawFilledTriangle(Point{-200, -250, 0.5f}, Point{2   00, 50, 0.5f}, Point{20, 250}, sf::Color::Green);
        //DrawLine(glm::vec2(-200, -250), glm::vec2(200, 50), sf::Color::Black);
        //DrawWireframeTriangle(Point{-200, -250}, Point{200, 50}, Point{20, 250}, sf::Color::Black);

        //DrawCube();
        //RenderObject(c.vertices, c.triangles);

        RenderScene(instances);

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

    temp = p0.h;
    p0.h = p1.h;
    p1.h = temp;
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
    std::vector<float> h02 = Interpolate(p0.y, p0.h, p2.y, p2.h);

           
    // std::cout << "--- h02 --- " << "\n";
    // for(int i = 0; i < h02.size(); i++)
    // {
    //     std::cout << "h02:" << h02[i] << "\n";
    // }
    

    
    // Concatenate short sides
    x01.pop_back();
    std::vector<float> x012;
    x012.insert(x012.end(), x01.begin(), x01.end());
    x012.insert(x012.end(), x12.begin(), x12.end());

    h01.pop_back();
    std::vector<float> h012;
    h012.insert(h012.end(), h01.begin(), h01.end());
    h012.insert(h012.end(), h12.begin(), h12.end());

    /*
    std::cout << "x01 size: " << x01.size() << "\n";
    std::cout << "x12 size: " << x12.size() << "\n";
    std::cout << "x02 size: " << x02.size() << "\n";
    std::cout << "x012 size: " << x012.size() << "\n";
    */

    std::vector<float> x_left;
    std::vector<float> x_right;
    std::vector<float> h_left;
    std::vector<float> h_right;

    // Determine which is left and which is right
    int m = static_cast<int>(floor(x012.size()/2));
    if(x02[m] < x012[m])
    {
        x_left = x02;
        h_left = h02;

        x_right = x012;
        h_right = h012;
    }else
    {
        x_left = x012;
        h_left = h012;

        x_right = x02;
        h_right = h02;
    }

    //std::cout << "x_left size: " << x_left.size() << "\n";
    //std::cout << "x_right size: " << x_right.size() << "\n";
    
    // Draw the horizontal segments
    for(float y = p0.y; y <= p2.y; y++)
    {
        float x_l = x_left[static_cast<int>(y - p0.y)];
        float x_r = x_right[static_cast<int>(y - p0.y)];

        //printf("h_left: %f\n", h_left[static_cast<int>(y - p0.y)]);
        //printf("h_right: %f\n", h_right[static_cast<int>(y - p0.y)]);
        std::vector<float> h_segment = Interpolate(x_l, h_left[static_cast<int>(y - p0.y)], x_r, h_right[static_cast<int>(y - p0.y)]);

        //printf("----------------------");
        for(float x = x_l; x <= x_r; x++)
        {   sf::Color modified_color = color;
            modified_color = ModifiedColor(modified_color, h_segment[x - x_l]);
            //printf("h_segment[x - x_l]: %f\n", h_segment[x - x_l]);
            PutPixel(window, pixel, static_cast<int>(x), static_cast<int>(y), modified_color);
        }
    }
}

sf::Color ModifiedColor(sf::Color color, float modifier)
{
    sf::Color return_value;
    //printf("%f\n", modifier);

    float r = color.r * modifier;
    float g = color.g * modifier;
    float b = color.b * modifier;

    r > 255 ? return_value.r = 255 : return_value.r = static_cast<uint8_t>(r);
    b > 255 ? return_value.b = 255 : return_value.b = static_cast<uint8_t>(b);
    g > 255 ? return_value.g = 255 : return_value.g = static_cast<uint8_t>(g);

    return return_value;
}

glm::vec3 ViewportToCanvas(float x, float y, float d)
{
    return glm::vec3(x * canvas_width/viewport_width, y * canvas_height/viewport_height, d);
}

Point ProjectVertex(Point v)
{
    glm::vec3 new_v = ViewportToCanvas(v.x * d/v.z, v.y * d/v.z, d);
    v.x = new_v.x;
    v.y = new_v.y;

    
    return v;
}

void DrawCube()
{
    glm::vec3 translation_vector = glm::vec3(-1.5f, 0.0f, 7.0f);
    std::vector<Point> vertices;
    vertices.push_back(Point{1, 1, 1} + translation_vector);
    vertices.push_back(Point{-1, 1, 1} + translation_vector);
    vertices.push_back(Point{-1, -1, 1} + translation_vector);
    vertices.push_back(Point{1, -1, 1} + translation_vector);
    vertices.push_back(Point{1, 1, -1} + translation_vector);
    vertices.push_back(Point{-1, 1, -1} + translation_vector);
    vertices.push_back(Point{-1, -1, -1} + translation_vector);
    vertices.push_back(Point{1, -1, -1} + translation_vector);


    std::vector<Triangle> triangles;
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

    RenderObject(vertices, triangles);
}

void RenderObject(std::vector<Point> vertices, std::vector<Triangle> triangles)
{
    std::vector<Point> projected;
    for(auto& v : vertices)
    {
        projected.push_back(ProjectVertex(v));
    }

    for(auto& t : triangles)
    {
        RenderTriangle(t, projected);
    }
}

void RenderTriangle(Triangle triangle, std::vector<Point>& projected)
{
    DrawWireframeTriangle(projected[triangle.point_indices[0]], projected[triangle.point_indices[1]], projected[triangle.point_indices[2]], triangle.color);
}

void RenderScene(std::vector<Instance> instances)
{
    for(auto& instance : instances)
    {
        RenderInstance(instance);
    }
}

void RenderInstance(Instance instance)
{
    std::vector<Point> projected;
    Model model = instance.model;
    for(auto& cube : model.cubes)
    {
        for(auto& v : cube.vertices)
        {
            projected.push_back(ProjectVertex(v));
        }

        for(auto& t : cube.triangles)
        {
            RenderTriangle(t, projected);
        }

        projected.clear();
    }

   
}
