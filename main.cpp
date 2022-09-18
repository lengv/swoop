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

double particle_error(const double dist, const double percentage, const double particle_dist=-1.0, const double max_dist=20)
{  
    // if particle_dist is set then there will be a consistent output of particle dist
    // max_dist is only used when particle_dist is set to negative

    // Check invariants
    assert(dist > 0.0);
    assert(0.0 <= percentage && percentage <= 1.0);
    assert(max_dist >= 0.0 && max_dist > dist);  // Not strictly necessary

    double failure = double_rand();
    double new_dist = dist;  // default
    if(failure < percentage)
    {
        if(particle_dist < 0.0)
        {
            new_dist = max_dist*double_rand();
        } else
        {
            new_dist = particle_dist;  // Ensure new_dist is never set to negative
        }
    }

    assert(new_dist > 0.0);  // Only compiled for debug mode
    return new_dist;
}

double laser_noise(const double distance, const double std, const double mean)
{
    std::default_random_engine generator;
    std::normal_distribution<double> dist(mean, std);
    double noise = dist(generator);

    double new_dist = noise*distance*distance + distance;  // Scale with square distance
    return new_dist;
}

Point Scene::moveDrone(const Vector& velocity)
{

}

// App

int main(int, char**) {
    std::cout << "Hello, world!\n";
    Point p1 = {1,2,3};
    std::cout << "x: " << p1.x << std::endl;

}
