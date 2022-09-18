#include <iostream>
#include <random>
#include <cstdlib>
#include <assert.h>
#include <math.h>

#include "include/scene.h"
#include "include/utils.h"
#include "include/drone.h"

#define LANDING_HEIGHT 1.5  // Height to initiate landing
#define FORWARD_SPEED 0.2
#define LANDING_SPEED 0.1
// #define TARGET_X scene::LANDING_X
#define TARGET_X scene::LANDING_BLOCK


int main(int, char**) {

    auto env = scene::Scene();
    env.applyError(4);
    auto r1 = env.laserReturn(utils::Point{2,0,10});
    auto r2 = env.laserReturn(utils::Point{6,0,10});

    std::cout << "Hello, world!\n";

    std::cout << "r1: " << r1 << " r2: " << r2 << std::endl;

    auto drone = Drone(utils::Point{0,0,15});
    drone.drone_forward(env, 6.0, FORWARD_SPEED);
    drone.drone_land(env, LANDING_SPEED, LANDING_HEIGHT);

    drone.print_log();
    env.print_log();

    return 0;
}
