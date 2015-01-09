/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __COMMON_HPP__
#define __COMMON_HPP__


#include "../definitions.hpp"


namespace Dodge {

class Vec2f;

namespace Math {


extern bool lineIntersect(const Vec2f& l1p1, const Vec2f& l1p2, const Vec2f& l2p1, const Vec2f& l2p2, Vec2f& p);

inline bool isBetween(float32_t a, float32_t b, float32_t c) {
   return !((a < b && a < c) || (a > b && a > c));
}


}
}


#endif
