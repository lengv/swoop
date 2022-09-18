#ifndef LASER_ERROR_H
#define LASER_ERROR_H

#include <random>
#include <cstdlib>

namespace laser_error
{
double particle_error(const double dist, const double percentage, const double particle_dist, const double max_dist);
double laser_noise(const double distance, const double std, const double mean);
}

#endif