#include <math/common.hpp>


namespace Dodge {
namespace Math {


//===========================================
// Math::lineIntersect
//
// Returns true if the two lines (not line segments) intersect.
//===========================================
bool lineIntersect(const Vec2f& l1p1, const Vec2f& l1p2, const Vec2f& l2p1, const Vec2f& l2p2, Vec2f& p) {
   double a = l1p1.x * l1p2.y - l1p1.y * l1p2.x;
   double b = l1p1.x - l1p2.x;
   double c = l2p1.x * l2p2.y - l2p1.y * l2p2.x;
   double d = l2p1.x - l2p2.x;
   double A = a * d - b * c;

   double a_ = l1p1.x - l1p2.x;
   b = l1p1.y - l1p2.y;
   double c_ = l2p1.x - l2p2.x;
   d = l2p1.y - l2p2.y;
   double B = a_ * d - b * c_;

   if (B == 0.0) return false;

   p.x = A / B;

   b = l1p1.y - l1p2.y;
   d = l2p1.y - l2p2.y;

   A = a * d - b * c;

   p.y = A / B;

   return true;
}


}
}
