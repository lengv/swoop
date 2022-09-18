#include "../include/scene.h"
#include "../include/utils.h"

namespace scene{
utils::Point Scene::intersect(const utils::Point& origin, const utils::Vector& ray)
{
    // Not useful in 2D assumption
    utils::Point intersection = {0,0,0};
    return intersection;
}

double Scene::laserReturn(const utils::Point& drone_pos)
{
    double height = drone_pos.z - plane_point.z;  // Assume default is the planes height
    if(box.start.x <= drone_pos.x && drone_pos.x <= box.end.x)
    {
        height = drone_pos.z - box.start.z;
    }

    // Return an error signal if height is below min range
    if(height < lidar_min_range)
        height = -1.0;
    else
        height = applyError(height);

    return height;
}

double Scene::applyError(const double height)
{
    // int modifier = std::rand();
    // Scene::laserReturn(utils::Point{1,1,5});

    return height;
}

utils::Point Scene::moveDrone(const utils::Vector& velocity)
{
    drone_pos.x = velocity.x;
    // this.drone_pos.y = velocity.y;  // Ignore y due to 2D assumption
    drone_pos.z = velocity.z;

    return drone_pos;
}

}