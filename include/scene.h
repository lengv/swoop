#ifndef SCENE_H
#define SCENE_H
#include "utils.h"
#include <vector>

#define START_LANDING_BLOCK 5.0
#define LANDING_BLOCK 6.0
#define END_LANDING_BLOCK 7.0
#define LANDING_BLOCK_HEIGHT 7.0
#define LANDING_X 4.5
#define LIDAR_MIN_RANGE 0.8

namespace scene{
class Scene {
  // Assume the scene is only 2D, xz plane
  public:
    Scene(){};
    // explicit Scene(const utils::Box& box);

    double min_dist=-10.0;
    double max_dist=10.0;
    double min_height=0.0;
    double max_height=20.0;
    double lidar_min_range=LIDAR_MIN_RANGE;
    utils::Point drone_pos=utils::Point{0,0,15};

    utils::Point intersect(const utils::Point& origin, const utils::Vector& ray);
    double laserReturn(const utils::Point& drone_pos);
    utils::Point moveDrone(const utils::Vector& velocity);
    double applyError(const double height);

    void print_log();

  private:
    utils::Vector plane_normal={0,0,1};  // Not useful in the 2D assumption
    utils::Point plane_point={0,0,0};
    utils::Box box={utils::Point{START_LANDING_BLOCK,0,LANDING_BLOCK_HEIGHT}, utils::Point{END_LANDING_BLOCK,0,LANDING_BLOCK_HEIGHT}};

    double apply_error(const double height);

    std::vector<utils::Point> drone_history;
    std::vector<double> laser_history;
    // std::vector<double> laser_no_error;
};
}
#endif