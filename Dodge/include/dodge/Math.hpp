/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __MATH_HPP__
#define __MATH_HPP__


#include "Vec2i.hpp"
#include "Vec2f.hpp"
#include "Vec3i.hpp"
#include "Vec3f.hpp"
#include "Rectangle.hpp"
#include "CompoundPoly.hpp"


namespace Dodge {
namespace Math {


bool compoundPolyIntersect(const Vec2f& pos1, const CompoundPoly& poly1, const Vec2f& pos2, const CompoundPoly& poly2);
bool polyIntersect(const Vec2f& pos1, const Poly& poly1, const Vec2f& pos2, const Poly& poly2);
bool compoundPolyOverlap(const Vec2f& pos1, const CompoundPoly& poly1, const Vec2f& pos2, const CompoundPoly& poly2);
bool polyOverlap(const Vec2f& pos1, const Poly& poly1, const Vec2f& pos2, const Poly& poly2);
bool lineIntersect(const Vec2f& l1p1, const Vec2f& l1p2, const Vec2f& l2p1, const Vec2f& l2p2, Vec2f& p);


//===========================================
// Math::isBetween
//===========================================
inline bool isBetween(double a, double b, double c) {
   return !((a < b && a < c) || (a > b && a > c));
}

//===========================================
// Math::lineSegmentIntersect
//===========================================
inline bool lineSegmentIntersect(const Vec2f& l1p1, const Vec2f& l1p2, const Vec2f& l2p1, const Vec2f& l2p2, Vec2f& p) {
   if (lineIntersect(l1p1, l1p2, l2p1, l2p2, p)) {
      if (isBetween(p.x, l1p1.x, l1p2.x) && isBetween(p.y, l1p1.y, l1p2.y)
         && isBetween(p.x, l2p1.x, l2p2.x) && isBetween(p.y, l2p1.y, l2p2.y)) return true;
   }
   return false;
}


}
}


#endif /*!__MATH_HPP__*/
