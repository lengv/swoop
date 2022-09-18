#include <iostream>
#include <random>
#include <cstdlib>
#include <assert.h>

#define LANDING_HEIGHT 1.5  // Height to initiate landing
#define FORWARD_SPEED 0.5
#define LANDING_SPEED 0.5

// scene.h

typedef struct triplet
{
    double x;
    double y;
    double z;
} Point;

typedef struct triplet Vector;

typedef struct Box
{
    // Simplifying here to make a box a rectangle, raising from the ground
    Point start;
    Point end;
} Box;


class Scene {
  // Assume the scene is only 2D, xz plane
  public:
    Scene();
    explicit Scene(const Box& box);

    double min_dist=-10.0;
    double max_dist=10.0;
    double min_height=0.0;
    double max_height=20.0;

    Point intersect(const Point& origin, const Vector& ray);
    double laserReturn(const Point& drone_pos);
    Point moveDrone(const Vector& velocity);
  private:
    Vector plane_normal={0,0,1};  // Not useful in the 2D assumption
    Point plane_point={0,0,0};
    Box box={Point{5,0,7}, Point{7,0,7}};

    double apply_error(const double height);
};

// scene.cpp

Point Scene::intersect(const Point& origin, const Vector& ray)
{
    // Not useful in 2D assumption
    Point intersection = {0,0,0};
    return intersection;
}

double Scene::laserReturn(const Point& drone_pos)
{
    double height = plane_point.z;  // Assume default is the planes height
    if(box.start.x <= drone_pos.x && drone_pos.x <= box.end.x)
    {
        height = box.start.z;
    }

    return apply_error(height);
}

double Scene::apply_error(const double height)
{
    int modifier = std::rand();

    return height;
}

double double_rand() {return static_cast <double> (std::rand()) / static_cast <double> (RAND_MAX);}



Point Scene::moveDrone(const Vector& velocity)
{

}

// App

int main(int, char**) {
    std::cout << "Hello, world!\n";
    Point p1 = {1,2,3};
    std::cout << "x: " << p1.x << std::endl;

}
