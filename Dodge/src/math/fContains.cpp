/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <memory>
#include <map>
#include <math/common.hpp>
#include <math/fContains.hpp>
#include <math/fIntersect.hpp>
#include <math/primitives/LineSegment.hpp>
#include <math/primitives/Ellipse.hpp>
#include <math/primitives/Polygon.hpp>
#include <math/Vec2f.hpp>
#include <globals.hpp>
#include <Exception.hpp>
#include <StringId.hpp>


using namespace std;


namespace Dodge {
namespace Math {
namespace {


typedef bool (*funcPtr_t)(const Primitive&, const Vec2f&, const Primitive&, const Vec2f&);
typedef std::map<std::pair<long, long>, funcPtr_t> dispatchTable_t;


//===========================================
// lsegLsegContains
//===========================================
bool lsegLsegContains(const Primitive& lseg1, const Vec2f& pos1, const Primitive& lseg2, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// lsegEllipseContains
//===========================================
bool lsegEllipseContains(const Primitive& lseg, const Vec2f& pos1, const Primitive& elps, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// lsegPolyContains
//===========================================
bool lsegPolyContains(const Primitive& lseg, const Vec2f& pos1, const Primitive& poly, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// ellipseEllipseContains
//===========================================
bool ellipseEllipseContains(const Primitive& elps1, const Vec2f& pos1, const Primitive& elps2, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// ellipsePolyContains
//===========================================
bool ellipsePolyContains(const Primitive& elps, const Vec2f& pos1, const Primitive& poly, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// polyPolyContains
//===========================================
bool polyPolyContains(const Primitive& poly1_, const Vec2f& pos1, const Primitive& poly2_, const Vec2f& pos2) {
   const Polygon& poly1 = static_cast<const Polygon&>(poly1_);
   const Polygon& poly2 = static_cast<const Polygon&>(poly2_);

   float32_t xMin1 = pos1.x + poly1.getMinimum().x;
   float32_t xMax1 = pos1.x + poly1.getMaximum().x;
   float32_t yMin1 = pos1.y + poly1.getMinimum().y;
   float32_t yMax1 = pos1.y + poly1.getMaximum().y;

   float32_t xMin2 = pos2.x + poly2.getMinimum().x;
   float32_t xMax2 = pos2.x + poly2.getMaximum().x;
   float32_t yMin2 = pos2.y + poly2.getMinimum().y;
   float32_t yMax2 = pos2.y + poly2.getMaximum().y;

   if (!(xMin1 <= xMax2 && xMax1 >= xMin2 && yMin1 <= yMax2 && yMax1 >= yMin2)) return false;

   Vec2f inf(xMax1 * 1.1, yMax1 * 1.1);

   for (int j = 0; j < poly2.getNumVertices(); ++j) {
      LineSegment l2(poly2.getVertex(j), poly2.getVertex((j + 1) % poly2.getNumVertices()));
      LineSegment l3(poly2.getVertex(j), inf);

      bool b = false;
      for (int i = 0; i < poly1.getNumVertices(); ++i) {
         LineSegment l1(poly1.getVertex(i), poly1.getVertex((i + 1) % poly1.getNumVertices()));
         if (intersect(l1, pos1, l2, pos2)) return false;

         if (intersect(l3, pos2, l1, pos1)) b = !b;
      }

      if (!b) return false;
   }

   return true;
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

   (*tbl)[makePair(internString("LineSegment"), internString("LineSegment"))] = &lsegLsegContains;
   (*tbl)[makePair(internString("LineSegment"), internString("Ellipse"))] = &lsegEllipseContains;
   (*tbl)[makePair(internString("LineSegment"), internString("Polygon"))] = &lsegPolyContains;

   (*tbl)[makePair(internString("Ellipse"), internString("Ellipse"))] = &ellipseEllipseContains;
   (*tbl)[makePair(internString("Ellipse"), internString("Polygon"))] = &ellipsePolyContains;

   (*tbl)[makePair(internString("Polygon"), internString("Polygon"))] = &polyPolyContains;

   return tbl;
}


}


//===========================================
// Math::contains
//===========================================
bool contains(const Primitive& obj1, const Vec2f& pos1, const Primitive& obj2, const Vec2f& pos2) {
   static unique_ptr<dispatchTable_t> tbl(initDispatchTable());

   dispatchTable_t::iterator it = tbl->find(makePair(obj1.typeId(), obj2.typeId()));
   if (it == tbl->end())
      throw Exception("Error in Math::contains(); Unrecognised primitive types", __FILE__, __LINE__);

   return it->second(obj1, pos1, obj2, pos2);
}


}
}
