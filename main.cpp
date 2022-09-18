#include <iostream>
#include <random>
#include <cstdlib>
#include <assert.h>
#include <math.h>

#include "include/scene.h"
#include "include/utils.h"

#define LANDING_HEIGHT 1.5  // Height to initiate landing
#define FORWARD_SPEED 0.2
#define LANDING_SPEED 0.1
#define TARGET_X scene::LANDING_X

utils::Point drone_forward(scene::Scene env, utils::Point& drone_pos, const double target_x, const double velocity)
{
    while(std::abs(drone_pos.x - target_x) > 0.01)
    {
        double next_vel = std::min(velocity, target_x - drone_pos.x);
        drone_pos.x += next_vel;
        auto height = env.laserReturn(drone_pos);
        std::cout << "drone_pos.x: " << drone_pos.x << ", height: " << height << std::endl;
    }

    return drone_pos;
}

utils::Point drone_land(scene::Scene env, utils::Point& drone_pos, const double landing_speed, const double landing_height)
{
    double remaining_height = drone_pos.z;
    double target_landing_speed = landing_speed;
    while(remaining_height > 0.01)
    {
        if(remaining_height < landing_height)
        {
            target_landing_speed = std::max(remaining_height/landing_height, 0.005);
        }
        drone_pos.z -= target_landing_speed;
        remaining_height -= target_landing_speed;
        std::cout << "drone_pos.z: " << drone_pos.z << ", dist: " << remaining_height << std::endl;
    }
    return drone_pos;
}

int main(int, char**) {

    auto env = scene::Scene();
    env.applyError(4);
    auto r1 = env.laserReturn(utils::Point{2,0,10});
    auto r2 = env.laserReturn(utils::Point{6,0,10});

    std::cout << "Hello, world!\n";

    std::cout << "r1: " << r1 << " r2: " << r2 << std::endl;

    utils::Point drone_pos{0,0,15};
    drone_forward(env, drone_pos, LANDING_X, FORWARD_SPEED);
    drone_land(env, drone_pos, LANDING_SPEED, LANDING_HEIGHT);

    return 0;
}
