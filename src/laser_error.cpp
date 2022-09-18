#include <assert.h>

#include "../include/laser_error.h"
#include "../include/utils.h"

namespace laser_error
{
double particle_error(const double dist, const double percentage, const double particle_dist=-1.0, const double max_dist=20)
{  
    // if particle_dist is set then there will be a consistent output of particle dist
    // max_dist is only used when particle_dist is set to negative

    // Check invariants
    assert(dist > 0.0);
    assert(0.0 <= percentage && percentage <= 1.0);
    assert(max_dist >= 0.0 && max_dist > dist);  // Not strictly necessary

    double failure = utils::double_rand();
    double new_dist = dist;  // default
    if(failure < percentage)
    {
        if(particle_dist < 0.0)
        {
            new_dist = max_dist*utils::double_rand();
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
}