#ifndef UTILS_H
#define UTILS_H
#include <cstdlib>

namespace utils
{
typedef struct triplet
{
    double x;
    //cppcheck-suppress unusedStructMember
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

inline double double_rand() {return static_cast <double> (std::rand()) / static_cast <double> (RAND_MAX);}
}
#endif