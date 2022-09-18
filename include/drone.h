#ifndef DRONE_H
#define DRONE_H

#include "utils.h"
#include "scene.h"
#include <vector>

class Drone
{
    public:
        explicit Drone(utils::Point pos);
        utils::Point pos;
        void drone_forward(scene::Scene env, const double target_x, const double velocity);
        void drone_land(scene::Scene env, const double landing_speed, const double landing_height);
        void print_log();
    private:
        size_t max_buffer=10;
        std::vector<double> laser_return_buffer;
        std::pair<double, double> laser_height(scene::Scene env);
        double cluster_proximity=0.1; // closeness of value
        double laser_per_call = 3;

        std::vector<double> height_history;
        std::vector<double> perceived_height_history;
        std::vector<double> laser_height_history;
        std::vector<double> velocity_history;
};

#endif