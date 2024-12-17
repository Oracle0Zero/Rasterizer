#pragma once

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

float d = 1.0f;

constexpr int canvas_width = 800;
constexpr int canvas_height = 800;

constexpr int viewport_width = 1;
constexpr int viewport_height = 1;

float scale_x = 1.0f, scale_y = 1.0f, scale_z = 1.0f;
float translate_x = 0.0f, translate_y = 0.0f, translate_z = 7.0f;

sf::RenderWindow window(sf::VideoMode(canvas_width, canvas_height), "Rasterizer");
sf::RectangleShape pixel(sf::Vector2f(1, 1));


Camera c;

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
Instance ClipInstanceAgainstPlane(Instance instance, Plane plane);
std::vector<Triangle>ClipTrianglesAgainstPlane(std::vector<Triangle> triangles, Plane plane);
std::vector<Triangle> ClipTriangle(Triangle triangle, Plane plane);
Scene ClipScene(Scene scene, std::vector<Plane> planes);
Instance ClipInstance(Instance instance, std::vector<Plane> planes);
void ProcessEvents();
Point Intersection(Point A, Point B, Plane plane);