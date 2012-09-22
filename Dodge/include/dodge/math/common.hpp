#ifndef __COMMON_HPP__
#define __COMMON_HPP__


#include "Vec2f.hpp"


namespace Dodge {
namespace Math {


bool lineIntersect(const Vec2f& l1p1, const Vec2f& l1p2, const Vec2f& l2p1, const Vec2f& l2p2, Vec2f& p);

inline bool isBetween(double a, double b, double c) {
   return !((a < b && a < c) || (a > b && a > c));
}


}
}


#endif
