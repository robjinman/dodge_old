/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <memory>
#include <map>
#include <math/common.hpp>
#include <math/fIntersect.hpp>
#include <math/primitives/LineSegment.hpp>
#include <math/primitives/Ellipse.hpp>
#include <math/primitives/Polygon.hpp>
#include <math/Vec2f.hpp>
#include <Exception.hpp>
#include <StringId.hpp>


using namespace std;


namespace Dodge {
namespace Math {
namespace {


typedef bool (*funcPtr_t)(const Primitive&, const Vec2f&, const Primitive&, const Vec2f&);
typedef std::map<std::pair<long, long>, funcPtr_t> dispatchTable_t;


//===========================================
// lsegLsegIntersect
//===========================================
bool lsegLsegIntersect(const Primitive& lseg1, const Vec2f& pos1, const Primitive& lseg2, const Vec2f& pos2) {
   const LineSegment& l1 = static_cast<const LineSegment&>(lseg1);
   const LineSegment& l2 = static_cast<const LineSegment&>(lseg2);

   const Vec2f& l1p1 = l1.getPoint1() + pos1;
   const Vec2f& l1p2 = l1.getPoint2() + pos1;
   const Vec2f& l2p1 = l2.getPoint1() + pos2;
   const Vec2f& l2p2 = l2.getPoint2() + pos2;

   Vec2f p;
   if (lineIntersect(l1p1, l1p2, l2p1, l2p2, p)) {
      if (isBetween(p.x, l1p1.x, l1p2.x) && isBetween(p.y, l1p1.y, l1p2.y)
         && isBetween(p.x, l2p1.x, l2p2.x) && isBetween(p.y, l2p1.y, l2p2.y)) return true;
   }

   return false;
}

//===========================================
// lsegEllipseIntersect
//===========================================
bool lsegEllipseIntersect(const Primitive& lseg, const Vec2f& pos1, const Primitive& elps, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// lsegPolyIntersect
//===========================================
bool lsegPolyIntersect(const Primitive& lseg, const Vec2f& pos1, const Primitive& poly, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// ellipseEllipseIntersect
//===========================================
bool ellipseEllipseIntersect(const Primitive& elps1, const Vec2f& pos1, const Primitive& elps2, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// ellipsePolyIntersect
//===========================================
bool ellipsePolyIntersect(const Primitive& elps, const Vec2f& pos1, const Primitive& poly, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// polyPolyIntersect
//===========================================
bool polyPolyIntersect(const Primitive& poly1_, const Vec2f& pos1, const Primitive& poly2_, const Vec2f& pos2) {
   const Polygon& poly1 = static_cast<const Polygon&>(poly1_);
   const Polygon& poly2 = static_cast<const Polygon&>(poly2_);

   double xMin1 = pos1.x + poly1.getMinimum().x;
   double xMax1 = pos1.x + poly1.getMaximum().x;
   double yMin1 = pos1.y + poly1.getMinimum().y;
   double yMax1 = pos1.y + poly1.getMaximum().y;

   double xMin2 = pos2.x + poly2.getMinimum().x;
   double xMax2 = pos2.x + poly2.getMaximum().x;
   double yMin2 = pos2.y + poly2.getMinimum().y;
   double yMax2 = pos2.y + poly2.getMaximum().y;

   if (!(xMin1 <= xMax2 && xMax1 >= xMin2 && yMin1 <= yMax2 && yMax1 >= yMin2)) return false;

   for (int i = 0; i < poly1.getNumVertices(); ++i) {
      LineSegment l1(poly1.getVertex(i) + pos1, poly1.getVertex((i + 1) % poly1.getNumVertices()) + pos1);

      for (int j = 0; j < poly2.getNumVertices(); ++j) {
         LineSegment l2(poly2.getVertex(j) + pos2, poly2.getVertex((j + 1) % poly2.getNumVertices()) + pos2);
         if (intersect(l1, Vec2f(0.f, 0.f), l2, Vec2f(0.f, 0.f))) return true;
      }
   }

   return false;
}

//===========================================
// makePair
//===========================================
pair<long, long> makePair(long a, long b) {
   return a < b ? pair<long, long>(a, b) : pair<long, long>(b, a);
}

//===========================================
// initDispatchTable
//===========================================
dispatchTable_t* initDispatchTable() {
   dispatchTable_t* tbl = new dispatchTable_t;

   (*tbl)[makePair(internString("LineSegment"), internString("LineSegment"))] = &lsegLsegIntersect;
   (*tbl)[makePair(internString("LineSegment"), internString("Ellipse"))] = &lsegEllipseIntersect;
   (*tbl)[makePair(internString("LineSegment"), internString("Polygon"))] = &lsegPolyIntersect;

   (*tbl)[makePair(internString("Ellipse"), internString("Ellipse"))] = &ellipseEllipseIntersect;
   (*tbl)[makePair(internString("Ellipse"), internString("Polygon"))] = &ellipsePolyIntersect;

   (*tbl)[makePair(internString("Polygon"), internString("Polygon"))] = &polyPolyIntersect;

   return tbl;
}


}


//===========================================
// Math::intersect
//===========================================
bool intersect(const Primitive& obj1, const Vec2f& pos1, const Primitive& obj2, const Vec2f& pos2) {
   static unique_ptr<dispatchTable_t> tbl(initDispatchTable());

   dispatchTable_t::iterator it = tbl->find(makePair(obj1.typeId(), obj2.typeId()));
   if (it == tbl->end())
      throw Exception("Error in Math::intersect(); unrecognised primitive types", __FILE__, __LINE__);

   return it->second(obj1, pos1, obj2, pos2);
}


}
}
