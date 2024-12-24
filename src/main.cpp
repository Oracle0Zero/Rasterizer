#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "../include/Camera.h"
#include "../include/Triangle.h"
#include "../include/Plane.h"
#include "../include/Model.h"
#include "../include/Instance.h"
#include "../include/Scene.h"
#include "../include/Utils.h"
#include "../include/imgui.h"
#include "../include/imgui-SFML.h" 

Scene s;
std::vector<Plane> planes;

sf::Image frameBuffer;
float depthBuffer[canvas_width][canvas_height];

bool observe_clipping = true;

int main()
{
    frameBuffer.create(canvas_width, canvas_height, sf::Color::White);

    sf::Texture frameTexture;
    sf::Sprite frameSprite;

    c.SetCameraPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    Cube c1;

    Instance instance_1;
    instance_1.cube = c1;
    instance_1.transform.translation = glm::vec3(0.0f, 0.0f, 7.0f);
    instance_1.transform.rotation_axis = glm::vec3(23.0f, 29.0f, 0.0f);
    instance_1.transform.scale_axis = glm::vec3(1.0f, 1.0f, 1.0f);

    Instance instance_2;
    instance_2.cube = c1;
    instance_2.transform.translation = glm::vec3(1.5f, 1.0f, 7.5f);
    instance_2.transform.rotation_axis = glm::vec3(0.0f, 76.0f, 23.0f);
    instance_2.transform.scale_axis = glm::vec3(1.0f, 1.0f, 1.0f);

    s.instances.push_back(instance_1);
    
    planes.push_back(Plane{glm::vec3(0, 0, 1), -d, "near"});
    planes.push_back(Plane{glm::vec3(1.0f/sqrt(2), 0, 1.0f/sqrt(2)), 0, "left"});
    planes.push_back(Plane{glm::vec3(-1.0f/sqrt(2), 0, 1.0f/sqrt(2)), 0, "right"});
    planes.push_back(Plane{glm::vec3(0, 1.0f/sqrt(2), 1.0f/sqrt(2)), 0, "bottom"});
    planes.push_back(Plane{glm::vec3(0, -1.0f/sqrt(2), 1.0f/sqrt(2)), 0, "top"});

    while (window.isOpen())
    {
        for(int i = 0; i < canvas_width; i++)
        {
            for(int j = 0; j < canvas_height; j++)
            {
                frameBuffer.setPixel(i, j, sf::Color::White);
            }
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ProcessEvents();


        ClearDepthBuffer(depthBuffer);
        window.clear(sf::Color::White);

        RenderScene(s.instances);

        frameTexture.loadFromImage(frameBuffer);
        frameSprite.setTexture(frameTexture);

        window.draw(frameSprite);

        window.display();
    }

    return 0;
}

void PutPixel(sf::RenderWindow& window, sf::RectangleShape& pixel, int x, int y, sf::Color color)
{
    float c_x = (float)canvas_width / 2 + x;
    float c_y = (float)canvas_height / 2 - y;

    unsigned int c_x_unsigned = static_cast<u_int>(c_x);
    unsigned int c_y_unsigned = static_cast<u_int>(c_y);

    if(c_x_unsigned >= 0 && c_x_unsigned < canvas_width)
    {
        if(c_y_unsigned >= 0 && c_y_unsigned < canvas_height)
        {
            frameBuffer.setPixel(c_x_unsigned, c_y_unsigned, color);
        }
    }
}

void PutPixel(sf::RenderWindow& window, sf::RectangleShape& pixel, int x, int y, float z, sf::Color color)
{
    float c_x = (float)canvas_width / 2 + x;
    float c_y = (float)canvas_height / 2 - y;

    unsigned int c_x_unsigned = static_cast<u_int>(c_x);
    unsigned int c_y_unsigned = static_cast<u_int>(c_y);

    if(c_x_unsigned >= 0 && c_x_unsigned < canvas_width)
    {
        if(c_y_unsigned >= 0 && c_y_unsigned < canvas_height)
        {
            if(z > depthBuffer[c_x_unsigned][c_y_unsigned])
            {
                frameBuffer.setPixel(c_x_unsigned, c_y_unsigned, color);
                depthBuffer[c_x_unsigned][c_y_unsigned] = z;
            }
        }
    }
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

    if(abs(dx) > abs(dy))
    {
        // Line is horizontal-ish
        // Make sure p0.x < p1.x
        if(p0.x > p1.x)
        {
            SwapPoints(p0, p1);
        }  
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

    temp = p0.z;
    p0.z = p1.z;
    p1.z = temp;

    temp = p0.h;
    p0.h = p1.h;
    p1.h = temp;
}

void DrawFilledTriangle(Point p0, Point p1, Point p2, sf::Color color)
{
    // Sort the points so that p0.y <= p1.y <= p2.y
    if(p1.y < p0.y) { SwapPoints(p1, p0);}
    if(p2.y < p0.y) { SwapPoints(p2, p0);}
    if(p2.y < p1.y) { SwapPoints(p2, p1);}

    // Compute the x,z coordinates and h values of the triangle edges
    std::vector<float> x01 = Interpolate(p0.y, p0.x, p1.y, p1.x);
    std::vector<float> z01 = Interpolate(p0.y, 1/p0.z, p1.y, 1/p1.z);
    std::vector<float> h01 = Interpolate(p0.y, p0.h, p1.y, p1.h);

    std::vector<float> x12 = Interpolate(p1.y, p1.x, p2.y, p2.x);
    std::vector<float> z12 = Interpolate(p1.y, 1/p1.z, p2.y, 1/p2.z);
    std::vector<float> h12 = Interpolate(p1.y, p1.h, p2.y, p2.h);

    std::vector<float> x02 = Interpolate(p0.y, p0.x, p2.y, p2.x);
    std::vector<float> z02 = Interpolate(p0.y, 1/p0.z, p2.y, 1/p2.z);
    std::vector<float> h02 = Interpolate(p0.y, p0.h, p2.y, p2.h);

    // Concatenate short sides
    x01.pop_back();
    std::vector<float> x012;
    x012.insert(x012.end(), x01.begin(), x01.end());
    x012.insert(x012.end(), x12.begin(), x12.end());

    z01.pop_back();
    std::vector<float> z012;
    z012.insert(z012.end(), z01.begin(), z01.end());
    z012.insert(z012.end(), z12.begin(), z12.end());

    h01.pop_back();
    std::vector<float> h012;
    h012.insert(h012.end(), h01.begin(), h01.end());
    h012.insert(h012.end(), h12.begin(), h12.end());

    std::vector<float> x_left;
    std::vector<float> x_right;
    std::vector<float> z_left;
    std::vector<float> z_right;
    std::vector<float> h_left;
    std::vector<float> h_right;

    // Determine which is left and which is right
    int m = static_cast<int>(floor(x012.size()/2)); 
    if(x02[m] < x012[m])
    {
        x_left = x02;
        z_left = z02;
        h_left = h02;

        x_right = x012;
        z_right = z012;
        h_right = h012;
    }else
    {
        x_left = x012;
        z_left = z012;
        h_left = h012;

        x_right = x02;
        z_right = z02;
        h_right = h02;
    }

    // Draw the horizontal segments
    for(float y = p0.y; y <= p2.y; y++)
    {
        float x_l = 0.0f, x_r = 0.0f;
        if(static_cast<int>(y - p0.y) >= 0 && static_cast<int>(y - p0.y) < x_left.size())
        {
            x_l = x_left[static_cast<int>(y - p0.y)];
        }

        if(static_cast<int>(y - p0.y) >= 0 && static_cast<int>(y - p0.y) < x_right.size())
        {
            x_r = x_right[static_cast<int>(y - p0.y)];
        }

        std::vector<float> h_segment;
        if(static_cast<int>(y - p0.y) >= 0 && static_cast<int>(y - p0.y) < h_left.size() && static_cast<int>(y - p0.y) < h_right.size())
        {
            h_segment = Interpolate(x_l, h_left[static_cast<int>(y - p0.y)], x_r, h_right[static_cast<int>(y - p0.y)]);
        }

        std::vector<float> z_segment;
        if(static_cast<int>(y - p0.y) >= 0 && static_cast<int>(y - p0.y) < z_left.size() && static_cast<int>(y - p0.y) < z_right.size())
        {
            z_segment = Interpolate(x_l, z_left[static_cast<int>(y - p0.y)], x_r, z_right[static_cast<int>(y - p0.y)]);
        }
        
        for(float x = x_l; x <= x_r; x++)
        {   sf::Color modified_color = color;

            if(static_cast<int>(x - x_l) >= 0 && static_cast<int>(x - x_l) < h_segment.size())
            {
                if(static_cast<int>(x - x_l) >= 0 && static_cast<int>(x - x_l) < z_segment.size())
                {
                    float z = z_segment[static_cast<int>(x - x_l)];

                    modified_color = ModifiedColor(color, h_segment[static_cast<int>(x - x_l)]);
                    PutPixel(window, pixel, static_cast<int>(x), static_cast<int>(y), z, modified_color);
                }
            }
        }
    }
}

sf::Color ModifiedColor(sf::Color color, float modifier)
{
    sf::Color return_value;

    float r = color.r * modifier;
    float g = color.g * modifier;
    float b = color.b * modifier;

    r > 255 ? return_value.r = 255 : return_value.r = static_cast<uint8_t>(r);
    b > 255 ? return_value.b = 255 : return_value.b = static_cast<uint8_t>(b);
    g > 255 ? return_value.g = 255 : return_value.g = static_cast<uint8_t>(g);

    return return_value;
}

glm::vec3 ViewportToCanvas(float x, float y, float z)
{
    return glm::vec3(x * canvas_width/viewport_width, y * canvas_height/viewport_height, z);
}

Point ProjectVertex(Point v)
{
    glm::vec3 new_v;

    new_v = ViewportToCanvas(v.x * d/v.z, v.y * d/v.z, v.z);

    v.x = new_v.x;
    v.y = new_v.y;

    return v;
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
        //RenderTriangle(t, projected);
    }
}

void RenderTriangle(Triangle triangle)
{
    DrawFilledTriangle(triangle.vertices[0], triangle.vertices[1], triangle.vertices[2], triangle.color);
}

void RenderScene(std::vector<Instance> instances)
{
    for(auto instance : instances)
    {
        RenderInstance(instance);
    }
}

void RenderInstance(Instance& instance)
{ 
   std::vector<Triangle> triangles_to_clip;
   for(auto t : instance.cube.triangles)
   {
        std::vector<Point> transformed_vertices;

        for(auto p : t.vertices)
        {
            if(observe_clipping)
            {
                Point model_transformed_vertex = ApplyTransform(p, instance.transform);
                transformed_vertices.push_back(model_transformed_vertex);
            }else
            {
                Point model_transformed_vertex = ApplyTransform(p, instance.transform);
                Point camera_transformed_vertex = ApplyCameraTransform(model_transformed_vertex, c.camera_transform);
                transformed_vertices.push_back(camera_transformed_vertex); 
            }

        }

        triangles_to_clip.push_back(Triangle(transformed_vertices, t.color, false));
   }

    // Calculate Center and Radius
    Point center{0.0f, 0.0f, 0.0f};
    int divider = 0;
    for(auto t : triangles_to_clip)
    {
        for(auto p : t.vertices)
        {
            center = center + p;
            divider++;
        }
    }
    
    instance.bounding_sphere_center.x = center.x / divider;
    instance.bounding_sphere_center.y = center.y / divider;
    instance.bounding_sphere_center.z = center.z / divider;

    Point difference_point = triangles_to_clip[0].vertices[0] - instance.bounding_sphere_center;
    glm::vec3 difference_vector(difference_point.x, difference_point.y, difference_point.z);

    instance.bounding_sphere_radius = glm::length(difference_vector);

    // Clipping
    instance.cube.triangles = triangles_to_clip;
    Instance clipped_instance = ClipInstance(instance, planes);

    std::vector<Triangle> triangles_to_render;
    if(!clipped_instance.null)
    {
        // Projection Tranformation
        for(auto t: clipped_instance.cube.triangles)
        {
            if(!t.null)
            {
                std::vector<Point> transformed_vertices;

                for(auto p : t.vertices)
                {
                    if(observe_clipping)
                    {
                        Point camera_transformed_vertex = ApplyCameraTransform(p, c.camera_transform);
                        Point perspective_transformed_vertex = ProjectVertex(camera_transformed_vertex);
                        transformed_vertices.push_back(perspective_transformed_vertex);
                    }else
                    {
                        Point perspective_transformed_vertex = ProjectVertex(p);
                        transformed_vertices.push_back(perspective_transformed_vertex);
                    }
                }

                triangles_to_render.push_back(Triangle(transformed_vertices, t.color, false));
            }
        }

        // Render Transformed Triangles
        for(auto t : triangles_to_render)
        {
            if(!t.null)
            {
                RenderTriangle(t);
            }
        }
    }

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

Scene ClipScene(Scene scene, std::vector<Plane> planes)
{
    std::vector<Instance> clipped_instances;
    for(auto& instance : scene.instances)
    {
        Instance clipped_instance = ClipInstance(instance, planes);
        if(clipped_instance.null == false)
        {
            clipped_instances.push_back(clipped_instance);
        }
    }

    Scene clipped_scene = scene;
    clipped_scene.instances = clipped_instances;

    return clipped_scene;
}

Instance ClipInstance(Instance instance, std::vector<Plane> planes)
{
    Instance clipped_instance = instance;
    for(auto plane : planes)
    {
        clipped_instance = ClipInstanceAgainstPlane(clipped_instance, plane);
        if(clipped_instance.null)
        {
            //std::cout << "Plane: " << plane.getName() << "\n";
            return clipped_instance;
        }
    }

    return clipped_instance;
}

Instance ClipInstanceAgainstPlane(Instance instance, Plane plane)
{
    Point instance_bounding_sphere_center(instance.bounding_sphere_center.x, \
    instance.bounding_sphere_center.y, instance.bounding_sphere_center.z);

    float d = SignedDistance(plane, instance_bounding_sphere_center);

    //std::cout << "------------------" << "\n";
    //std::cout << "Plane: " << plane.getName() << "\n";
    //std::cout << "d: " << d << "\n";

    Instance clipped_instance = instance;   
    if(d > instance.bounding_sphere_radius)
    {
        //std::cout << "Sphere is Inside" << "\n";
        instance.null = false;
        return instance;
    }else if(d < -instance.bounding_sphere_radius)
    {
        //std::cout << "Sphere is Outside" << "\n";
        instance.null = true;
        return instance;
    }
    else
    {
        //std::cout << "Sphere is In-Between" << "\n";
        clipped_instance.cube.triangles = ClipTrianglesAgainstPlane(instance.cube.triangles, plane);
    }
    
    return clipped_instance;
}

std::vector<Triangle>ClipTrianglesAgainstPlane(std::vector<Triangle> triangles, Plane plane)
{
    std::vector<Triangle> clipped_triangles_return;
    //std::cout << "------------------" << "\n";
    //std::cout << "Plane: " << plane.getName() << "\n";
    for(auto triangle : triangles)
    {
        std::vector<Triangle> clipped_triangles = ClipTriangle(triangle, plane);
        
        for(auto clipped_triangle : clipped_triangles)
        {
            //std::cout << "clipped_triangle.null: " << clipped_triangle.null << "\n";
            clipped_triangles_return.push_back(clipped_triangle);
        }
    }
    //std::cout << "clipped_triangles_return.size(): " << clipped_triangles_return.size() << "\n";
    return clipped_triangles_return;
}

std::vector<Triangle> ClipTriangle(Triangle triangle, Plane plane)
{
    float d0 = SignedDistance(plane, triangle.vertices[0]);
    float d1 = SignedDistance(plane, triangle.vertices[1]);
    float d2 = SignedDistance(plane, triangle.vertices[2]);

    
    //std::cout << "------------------" << "\n";
    //std::cout << "Plane: " << plane.getName() << "\n";
    //std::cout << "d0: " << d0 << "\n";
    //std::cout << "d1: " << d1 << "\n";
    //std::cout << "d2: " << d2 << "\n";
    

    std::vector<Triangle> clipped_triangles;
    if(d0 >= 0 && d1 >= 0 && d2 >= 0)
    {
        
        clipped_triangles.push_back(triangle);
        return clipped_triangles;

    }else if(d0 < 0 && d1 < 0 && d2 < 0)
    {
        triangle.null = true;
        clipped_triangles.push_back(triangle);

        return clipped_triangles;
    }
    /*
    else if(d0 > 0 && d1 < 0 && d2 < 0)
    {
        std::cout << "--------------------------" << "\n";
        std::cout << "Plane: " << plane.getName() << "\n";
        Point new_v1 = Intersection(triangle.vertices[0], triangle.vertices[1], plane);
        Point new_v2 = Intersection(triangle.vertices[0], triangle.vertices[2], plane);

        triangle.vertices[1] = new_v1;
        triangle.vertices[2] = new_v2;

        clipped_triangles.push_back(triangle);

        return clipped_triangles;

    }else if(d1 > 0 && d0 < 0 && d2 < 0)
    {
        Point new_v0 = Intersection(triangle.vertices[1], triangle.vertices[0], plane);
        Point new_v2 = Intersection(triangle.vertices[1], triangle.vertices[2], plane);

        triangle.vertices[0] = new_v0;
        triangle.vertices[2] = new_v2;

        clipped_triangles.push_back(triangle);

        return clipped_triangles;

    }else if(d2 > 0 && d0 < 0 && d1 < 0)
    {
        Point new_v0 = Intersection(triangle.vertices[2], triangle.vertices[0], plane);
        Point new_v1 = Intersection(triangle.vertices[2], triangle.vertices[1], plane);

        triangle.vertices[0] = new_v0;
        triangle.vertices[1] = new_v1;

        clipped_triangles.push_back(triangle);

        return clipped_triangles;

    }
    
    else if(d0 < 0 && d1 > 0 && d2 > 0)
    {
        
        Point new_v1 = Intersection(triangle.vertices[1], triangle.vertices[0], plane);
        Point new_v2 = Intersection(triangle.vertices[2], triangle.vertices[0], plane);

        Triangle clipped_triangle_1 = triangle;
        clipped_triangle_1.vertices[0] = new_v1;
    
        Triangle clipped_triangle_2 = triangle;
        clipped_triangle_2.vertices[0] = new_v1;
        clipped_triangle_2.vertices[1] = new_v2;
        clipped_triangle_2.vertices[2] = new_v2;

        clipped_triangles.push_back(clipped_triangle_1);
        clipped_triangles.push_back(clipped_triangle_2);

        return clipped_triangles;

    }else if(d1 < 0 && d0 > 0 && d2 > 0)
    {
        Point new_v0 = Intersection(triangle.vertices[0], triangle.vertices[1], plane);
        Point new_v2 = Intersection(triangle.vertices[2], triangle.vertices[1], plane);

        Triangle clipped_triangle_1 = triangle;
        clipped_triangle_1.vertices[0] = new_v0;
    
        Triangle clipped_triangle_2 = triangle;
        clipped_triangle_2.vertices[0] = new_v0;
        clipped_triangle_2.vertices[1] = new_v2;
        clipped_triangle_2.vertices[2] = new_v2;

        clipped_triangles.push_back(clipped_triangle_1);
        clipped_triangles.push_back(clipped_triangle_2);

        return clipped_triangles;

    }else if(d2 < 0 && d0 > 0 && d1 > 0)
    {
        Point new_v0 = Intersection(triangle.vertices[0], triangle.vertices[2], plane);
        Point new_v1 = Intersection(triangle.vertices[1], triangle.vertices[2], plane);

        Triangle clipped_triangle_1 = triangle;
        clipped_triangle_1.vertices[0] = new_v0;
    
        Triangle clipped_triangle_2 = triangle;
        clipped_triangle_2.vertices[0] = new_v0;
        clipped_triangle_2.vertices[1] = new_v1;
        clipped_triangle_2.vertices[2] = new_v1;

        clipped_triangles.push_back(clipped_triangle_1);
        clipped_triangles.push_back(clipped_triangle_2);

        return clipped_triangles;
    }
    */
    
    
    clipped_triangles.push_back(triangle);
    return clipped_triangles;
}

void ProcessEvents()
{
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
            translate_x -= 0.005f;
            s.instances[0].transform.translation = glm::vec3(translate_x, translate_y, translate_z);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            translate_x += 0.005f;
            s.instances[0].transform.translation = glm::vec3(translate_x, translate_y, translate_z);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            translate_y += 0.005f;
            s.instances[0].transform.translation = glm::vec3(translate_x, translate_y, translate_z);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            translate_y -= 0.005f;
            s.instances[0].transform.translation = glm::vec3(translate_x, translate_y, translate_z);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
        {
            translate_z += 0.005f;
            s.instances[0].transform.translation = glm::vec3(translate_x, translate_y, translate_z);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
        {
            translate_z -= 0.005f;
            s.instances[0].transform.translation = glm::vec3(translate_x, translate_y, translate_z);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            rotate_x = s.instances[0].transform.rotation_axis.x + 0.5f;
            s.instances[0].transform.rotation_axis = glm::vec3(rotate_x, rotate_y, rotate_z);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            rotate_y = s.instances[0].transform.rotation_axis.y + 0.5f;
            s.instances[0].transform.rotation_axis = glm::vec3(rotate_x, rotate_y, rotate_z);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            rotate_z = s.instances[0].transform.rotation_axis.z + 0.5f;
            s.instances[0].transform.rotation_axis = glm::vec3(rotate_x, rotate_y, rotate_z);
        }
}

Point Intersection(Point A, Point B, Plane plane)
{
    Point new_b;
    glm::vec3 vector_new_b;
    glm::vec3 vector_A(A.x, A.y, A.z);
    glm::vec3 vector_B(B.x, B.y, B.z);
    glm::vec3 difference_vector = vector_B - vector_A;
    float D = plane.GetDistanceFromOrigin();
    glm::vec3 plane_normal = plane.GetNormal();

    float t = (-1*D - glm::dot(plane_normal, vector_A)) / glm::dot(plane_normal, difference_vector);

    vector_new_b = vector_A + t*(difference_vector);

    new_b.x = vector_new_b.x;
    new_b.y = vector_new_b.y;
    new_b.x = vector_new_b.z;

    return new_b;
}

void ClearDepthBuffer(float depthBuffer[canvas_width][canvas_height])
{
    for(int i = 0; i < canvas_width; i++)
    {
        for(int j = 0; j < canvas_height; j++)
        {
            depthBuffer[i][j] = 0;
        }
    }
}



