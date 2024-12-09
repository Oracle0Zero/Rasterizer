#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Camera.h"
#include "Triangle.h"
#include "Plane.h"
#include "Model.h"
#include "Point.h"
#include "Instance.h"
#include "Scene.h"

constexpr int canvas_width = 800;
constexpr int canvas_height = 800;

constexpr int viewport_width = 1;
constexpr int viewport_height = 1;

float d = 1.0f;

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
Point ApplyTransform(Point v, Transform transform);
Point Scale(Point v, glm::vec3 scale_axis);
Point Rotate(Point v, glm::vec3 rotation_axis);
Point Translate(Point v, glm::vec3 translation_vector);
Point ApplyCameraTransform(Point v, Transform transform);
float SignedDistance(Plane plane, Point vertex);
float SignedDistance(Plane plane, Point vertex);

sf::RenderWindow window(sf::VideoMode(canvas_width, canvas_height), "Rasterizer");
sf::RectangleShape pixel(sf::Vector2f(1, 1));


Camera c;

int main()
{
    c.SetCameraPosition(glm::vec3(0.0f, 0.0f, -d));
    Cube c1;

    Model model;
    model.cubes.push_back(c1);

    Instance instance_1;
    instance_1.model = model;
    instance_1.transform.translation = glm::vec3(1.0f, 2.0f, 7.0f);
    instance_1.transform.rotation_axis = glm::vec3(23.0f, 29.0f, 0.0f);
    instance_1.transform.scale_axis = glm::vec3(2.0f, 1.5f, 1.0f);
    
    Instance instance_2;
    instance_2.model = model;
    instance_2.transform.translation = glm::vec3(-1.25f, -2.0f, 7.5f);
    instance_2.transform.rotation_axis = glm::vec3(0.0f, 76.0f, 23.0f);
    instance_2.transform.scale_axis = glm::vec3(3.0f, 1.0f, 1.0f);
    

    Scene s;
    s.instances.push_back(instance_1);
    s.instances.push_back(instance_2);

    std::vector<Plane> planes;
    planes.push_back(Plane{glm::vec3(0, 0, 1), -d});
    planes.push_back(Plane{glm::vec3(1.0f/sqrt(2), 0, 1.0f/sqrt(2)), 0});
    planes.push_back(Plane{glm::vec3(-1.0f/sqrt(2), 0, 1.0f/sqrt(2)), 0});
    planes.push_back(Plane{glm::vec3(0, 1.0f/sqrt(2), 1.0f/sqrt(2)), 0});
    planes.push_back(Plane{glm::vec3(0, -1.0f/sqrt(2), 1.0f/sqrt(2)), 0});

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            c.camera_transform.translation.z += 0.05f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            c.camera_transform.translation.z -= 0.05f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            c.camera_transform.translation.x -= 0.05f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            c.camera_transform.translation.x += 0.05f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            c.camera_transform.rotation_axis.y += 0.5f;
        }

        window.clear(sf::Color::White);


        RenderScene(s.instances);

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
    std::vector<Point> model_transformed;
    //std::vector<Point> view_transformed;
    std::vector<Point> projected;
    Model model = instance.model;
    for(auto& cube : model.cubes)
    {
        for(auto& v : cube.vertices)
        {

            Point v_model = ApplyTransform(v, instance.transform);
            model_transformed.push_back(v_model);

            Point v_view = ApplyCameraTransform(v_model, c.camera_transform);
            projected.push_back(ProjectVertex(v_view));
        }

        Point center{0.0f, 0.0f, 0.0f};
        float far_point_distance = 0.0f;
        for(auto& p : model_transformed)
        {
            
            center = center + p;
            
        }

        cube.bounding_sphere_center.x = center.x / cube.vertices.size();
        cube.bounding_sphere_center.y = center.y / cube.vertices.size();
        cube.bounding_sphere_center.z = center.z / cube.vertices.size();
        cube.bounding_sphere_center.h = center.h / cube.vertices.size();

        cube.bounding_shpere_radius = (model_transformed[0] - cube.bounding_sphere_center).Magnitude();
        
        //printf("%f\n", cube.bounding_shere_radius);

        for(auto& t : cube.triangles)
        {
            RenderTriangle(t, projected);
        }

        model_transformed.clear();
        projected.clear();
    }


    Point center{0.0f, 0.0f, 0.0f};
    // Calculate Instance Center and Radis
    for(auto& cube : model.cubes)
    {
        center = center + cube.bounding_sphere_center;
    }

    instance.bounding_sphere_center.x = center.x / instance.model.cubes.size();
    instance.bounding_sphere_center.y = center.y / instance.model.cubes.size();
    instance.bounding_sphere_center.z = center.z / instance.model.cubes.size();

    //instance.bounding_shpere_radius = (model.cubes[0].bounding_sphere_center - instance.bounding_sphere_center).Magnitude();
}

Point ApplyTransform(Point v, Transform transform)
{
    

    Point v_scaled = Scale(v, transform.scale_axis);
    Point v_rotated = Rotate(v_scaled, transform.rotation_axis);
    Point v_translated = Translate(v_rotated, transform.translation);

    return v_translated;
}

Point ApplyCameraTransform(Point v, Transform transform)
{
    Point v_scaled = Scale(v, transform.scale_axis);
    Point v_rotated = Rotate(v_scaled, -1.0f*transform.rotation_axis);
    Point v_translated = Translate(v_rotated, -1.0f*transform.translation);   


    return v_translated;
}

Point Scale(Point v, glm::vec3 scale_axis)
{
    glm::vec4 v_vertex(v.x, v.y, v.z, 1.0f);
    glm::vec4 v_scaled = glm::scale(glm::mat4(1.0f), scale_axis) * v_vertex;

    Point p_scaled(v_scaled.x, v_scaled.y, v_scaled.z);

    return p_scaled;
}

Point Rotate(Point v, glm::vec3 rotation_axis)
{
    glm::vec4 v_vertex(v.x, v.y, v.z, 1.0f);
    glm::vec4 v_rotated_x = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_axis.x), glm::vec3(1.0f, 0.0f, 0.0f)) * v_vertex;
    glm::vec4 v_rotated_y = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_axis.y), glm::vec3(0.0f, 1.0f, 0.0f)) * v_rotated_x;
    glm::vec4 v_rotated_z = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_axis.z), glm::vec3(0.0f, 0.0f, 1.0f)) * v_rotated_y;

    Point p_rotated(v_rotated_z.x, v_rotated_z.y, v_rotated_z.z);

    return p_rotated;
}

Point Translate(Point v, glm::vec3 translation_vector)
{
    glm::vec4 v_vertex(v.x, v.y, v.z, 1.0f);
    glm::vec3 v_translated = glm::translate(glm::mat4(1.0f), translation_vector) * v_vertex;

    Point p_translated(v_translated.x, v_translated.y, v_translated.z);

    return p_translated;
}


float SignedDistance(Plane plane, Point vertex)
{
    glm::vec3 normal = plane.GetNormal();

    return (vertex.x*normal.x) + (vertex.y*normal.y) + (vertex.z*normal.z) + plane.GetDistanceFromOrigin();
}

Instance ClipInstanceAgainstPlane(Instance instance, Plane plane)
{
    Point instance_bounding_sphere_center(instance.bounding_sphere_center.x, instance.bounding_sphere_center.y, instance.bounding_sphere_center.z);
    float d = SignedDistance(plane, instance_bounding_sphere_center);
}


