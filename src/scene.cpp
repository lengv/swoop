#include <iostream>
#include <fstream>
#include "../include/scene.h"
#include "../include/utils.h"
#include "../include/laser_error.h"

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

    if(height < lidar_min_range)
        height = -1.0;

    laser_history.push_back(height);

    return height;
}

double Scene::applyError(const double height)
{
    // int modifier = std::rand();
    // Scene::laserReturn(utils::Point{1,1,5});
    double new_height;
    new_height = laser_error::particle_error(height, ERROR_RATE, height/2.0, 20);
    new_height = laser_error::laser_noise(new_height, 0.01, 0.0);

    return new_height;
}

utils::Point Scene::moveDrone(const utils::Vector& velocity)
{
    drone_pos.x += velocity.x;
    // this.drone_pos.y = velocity.y;  // Ignore y due to 2D assumption
    drone_pos.z += velocity.z;

    drone_history.push_back(utils::Point(drone_pos));

    return drone_pos;
}

void Scene::print_log()
{
    std::cout << "pos size: " << drone_history.size() << ", laser size: " << laser_history.size() <<std::endl;

    std::ofstream logfile;
    logfile.open ("scene_log.txt");
    for(auto pos : drone_history)
    {
        logfile << pos.x << "," << pos.y << "," << pos.z << std::endl;
    }
    logfile.close();
    std::ofstream logfile_laser;
    logfile_laser.open ("scene_laser_log.txt");
    // std::cout << "===== Laser history =====" <<std::endl;
    for(auto val : laser_history)
    {
        logfile_laser << val <<std::endl;
    }
    logfile_laser.close();
}
}