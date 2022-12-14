#include "../include/drone.h"
#include <cstdlib>
#include <math.h>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <fstream>

Drone::Drone(utils::Point pos) : pos(pos) {perceived_height=pos.z;};

double average(const std::vector<double>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), 0.0 )/ static_cast<double>(vec.size());
}

bool Longest(const std::vector<double> &A, const std::vector<double> &B)
{
    return A.size() < B.size();
}

void Drone::drone_forward(scene::Scene& env, const double target_x, const double velocity)
{
    while(std::abs(pos.x - target_x) > 0.01)
    {
        double next_vel = std::min(velocity, target_x - pos.x);
        pos.x += next_vel;
        env.moveDrone(utils::Vector{velocity, 0, 0});
        auto height = env.laserReturn(pos);
        std::cout << "drone_pos.x: " << pos.x << ", height: " << height << std::endl;

        fuse_height(env, 0);
    }

    return;
}

void Drone::drone_land(scene::Scene& env, const double landing_speed, const double landing_height)
{
    // double remaining_height = pos.z;  // dead reckoning
    // double perceived_height = pos.z;  // EKF like value
    double target_landing_speed = landing_speed;

    bool perceived_height_flag=true;
    std::vector<double> last_heights;
    while(perceived_height_flag)
    {
        fuse_height(env, 0);
        last_heights.push_back(perceived_height);
        if(last_heights.size() > 3)
        {
            last_heights.erase(last_heights.begin());
            double avg = average(last_heights);
            if(std::abs(perceived_height - avg) < 0.1)
                perceived_height_flag=false;
            std::cout << "waiting to land: " <<  avg << std::endl;
        }
        env.moveDrone(utils::Vector{0,0,0});
    }

    while(perceived_height > 0.01)
    {
        if(perceived_height < landing_height)
        {
            target_landing_speed = landing_speed*std::max(perceived_height/landing_height, 0.005);
        }
        pos.z -= target_landing_speed;
        env.moveDrone(utils::Vector{0, 0, -target_landing_speed});  // Track it in the sim
        // remaining_height -= target_landing_speed;
        fuse_height(env, target_landing_speed);
        // auto ls_data = laser_height(env);
        // double laser_weight = 0.5 * ls_data.second;
        // perceived_height = remaining_height*(1-laser_weight) + ls_data.first*(laser_weight);
        // remaining_height = perceived_height;
        // // std::cout << "drone_pos.z: " << pos.z << ", dist: " << remaining_height << " laser_height: " << ls_data.first <<" perceived_height: " << perceived_height << std::endl;

        // height_history.push_back(remaining_height);
        // perceived_height_history.push_back(perceived_height);
        // laser_height_history.push_back(ls_data.first);
        // velocity_history.push_back(target_landing_speed);
    }
    return;
}

void Drone::fuse_height(scene::Scene& env, double velocity_z)
{
    auto ls_data = laser_height(env);
    double height_diff_factor = std::abs(perceived_height - ls_data.first)/perceived_height/2;
    double laser_weight = 0.5 * (ls_data.second + height_diff_factor);

    if(ls_data.first<0.0)
        laser_weight = 0;
    // if(std::abs(perceived_height - ls_data.first) >  )
    

    height_history.push_back(perceived_height);
    perceived_height = (perceived_height-velocity_z)*(1-laser_weight) + ls_data.first*(laser_weight);
    // std::cout << "drone_pos.z: " << pos.z << ", dist: " << remaining_height << " laser_height: " << ls_data.first <<" perceived_height: " << perceived_height << std::endl;
    perceived_height_history.push_back(perceived_height);
    laser_height_history.push_back(ls_data.first);
    velocity_history.push_back(velocity_z);
}

std::pair<double, double> Drone::laser_height(scene::Scene& env)
{
    // Get laser returns, simulate multiple returns
    for(size_t laser_itr=0; laser_itr < laser_per_call; ++laser_itr)
    {
        double val = env.laserReturn(pos);
        // std::cout << " == pos ==" << pos.z << "val: " << val << std::endl;

        if(laser_return_buffer.size() >= max_buffer) 
        {
            laser_return_buffer.erase(laser_return_buffer.begin());  // Remove the oldest
        }
        laser_return_buffer.push_back(val);        
    }

    // Calc clusters
    std::vector<std::vector<double>> clusters;
    for(double ls_val: laser_return_buffer)
        {
            if(clusters.size() == 0)
            {
                clusters.push_back(std::vector<double>(1, ls_val));
            }
            else
            {
                bool clust_found = false;
                // Make a new cluster if outside of the range
                for(std::vector<double> clust : clusters)
                {
                    double avg = average(clust);
                    if(std::abs(avg-ls_val) < avg*cluster_proximity)
                    {
                        // Assumes clusters are fairly distinct, don't do additional passes to combine
                        clust.push_back(ls_val);
                        clust_found = true;
                        break;
                    }
                }
                if(!clust_found)
                    clusters.push_back(std::vector<double>(1, ls_val));
            }
        }
    
    std::vector<std::vector<double>>::iterator largest_clust_it= std::max_element(clusters.begin(),clusters.end(),Longest);
    size_t largest_ind = largest_clust_it - clusters.begin();
    std::vector<double> avgs;
    avgs.resize(clusters.size());    
    std::transform(clusters.begin(), clusters.end(), avgs.begin(), average);

    // Confidence is reported as the porportion of data that is valid compared to buffer size
    double confidence;
    if(avgs[largest_ind] == -1)
        confidence=0;
    else
        confidence = static_cast<double>(clusters[largest_ind].size())/static_cast<double>(max_buffer);
    
    return std::pair<double, double>(avgs[largest_ind], confidence);
}

void Drone::print_log()
{
    std::ofstream logfile;
    logfile.open("drone_log.txt");
    for(size_t i=0; i < height_history.size(); ++i)
    {
        logfile << height_history[i] << "," << perceived_height_history[i] << "," << laser_height_history[i]<< "," << velocity_history[i] << std::endl;
    }
    logfile.close();
}