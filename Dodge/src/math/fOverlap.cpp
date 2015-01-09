/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <memory>
#include <map>
#include <math/common.hpp>
#include <math/fOverlap.hpp>
#include <math/shapes/LineSegment.hpp>
#include <math/shapes/Ellipse.hpp>
#include <math/shapes/Polygon.hpp>
#include <math/Vec2f.hpp>
#include <globals.hpp>
#include <Exception.hpp>
#include <StringId.hpp>


using namespace std;


namespace Dodge {
namespace Math {
namespace {


typedef bool (*funcPtr_t)(const Shape&, const Vec2f&, const Shape&, const Vec2f&);
typedef std::map<std::pair<long, long>, funcPtr_t> dispatchTable_t;


//===========================================
// lsegLsegOverlap
//===========================================
bool lsegLsegOverlap(const Shape& lseg1, const Vec2f& pos1, const Shape& lseg2, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// lsegEllipseOverlap
//===========================================
bool lsegEllipseOverlap(const Shape& lseg, const Vec2f& pos1, const Shape& elps, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// lsegPolyOverlap
//===========================================
bool lsegPolyOverlap(const Shape& lseg, const Vec2f& pos1, const Shape& poly, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// ellipseEllipseOverlap
//===========================================
bool ellipseEllipseOverlap(const Shape& elps1, const Vec2f& pos1, const Shape& elps2, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// ellipsePolyOverlap
//===========================================
bool ellipsePolyOverlap(const Shape& elps, const Vec2f& pos1, const Shape& poly, const Vec2f& pos2) {
   // TODO
   return false;
}

//===========================================
// polyPolyOverlap
//===========================================
bool polyPolyOverlap(const Shape& poly1_, const Vec2f& pos1, const Shape& poly2_, const Vec2f& pos2) {
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

   int nLines = poly1.getNumVertices() + poly2.getNumVertices();

   StackAllocator::marker_t marker = gGetMemStack().getMarker();
   Vec2f* lines = static_cast<Vec2f*>(gGetMemStack().alloc(sizeof(Vec2f) * nLines));

   for (int i = 0; i < poly1.getNumVertices(); ++i) {
      Vec2f line = (poly1.getVertex(i) - poly1.getVertex((i + 1) % poly1.getNumVertices())) + pos1;

      // Normalise
      float32_t sf = sqrt(line.x * line.x + line.y * line.y);
      line.x /= sf;
      line.y /= sf;

      lines[i] = line;
   }

   for (int i = 0; i < poly2.getNumVertices(); ++i) {
      Vec2f line = (poly2.getVertex(i) - poly2.getVertex((i + 1) % poly2.getNumVertices())) + pos2;

      // Normalise
      float32_t sf = sqrt(line.x * line.x + line.y * line.y);
      line.x /= sf;
      line.y /= sf;

      lines[poly1.getNumVertices() + i] = line;
   }

   for (int l = 0; l < nLines; l++) {
      float32_t p1Min = 0, p1Max = 0, p2Min = 0, p2Max = 0;

      for (int i = 0; i < poly1.getNumVertices(); ++i) {

         // Projected point
         float32_t pt = lines[l].x * (pos1.x + poly1.getVertex(i).x) + lines[l].y * (pos1.y + poly1.getVertex(i).y);

         if (i == 0) {
            p1Min = pt;
            p1Max = pt;
         }

         if (pt < p1Min) p1Min = pt;
         if (pt > p1Max) p1Max = pt;
      }

      for (int i = 0; i < poly2.getNumVertices(); ++i) {

         // Projected point
         float32_t pt = lines[l].x * (pos2.x + poly2.getVertex(i).x) + lines[l].y * (pos2.y + poly2.getVertex(i).y);

         if (i == 0) {
            p2Min = pt;
            p2Max = pt;
         }

         if (pt < p2Min) p2Min = pt;
         if (pt > p2Max) p2Max = pt;
      }

      if (p1Max <= p2Min || p2Max <= p1Min) {
         gGetMemStack().freeToMarker(marker);
         return false;
      }
   }

   gGetMemStack().freeToMarker(marker);
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

   (*tbl)[makePair(internString("LineSegment"), internString("LineSegment"))] = &lsegLsegOverlap;
   (*tbl)[makePair(internString("LineSegment"), internString("Ellipse"))] = &lsegEllipseOverlap;
   (*tbl)[makePair(internString("LineSegment"), internString("Polygon"))] = &lsegPolyOverlap;

   (*tbl)[makePair(internString("Ellipse"), internString("Ellipse"))] = &ellipseEllipseOverlap;
   (*tbl)[makePair(internString("Ellipse"), internString("Polygon"))] = &ellipsePolyOverlap;

   (*tbl)[makePair(internString("Polygon"), internString("Polygon"))] = &polyPolyOverlap;

   return tbl;
}


}


//===========================================
// Math::overlap
//===========================================
bool overlap(const Shape& obj1, const Vec2f& pos1, const Shape& obj2, const Vec2f& pos2) {
   static unique_ptr<dispatchTable_t> tbl(initDispatchTable());

   dispatchTable_t::iterator it = tbl->find(makePair(obj1.typeId(), obj2.typeId()));
   if (it == tbl->end())
      throw Exception("Error in Math::overlap(); unrecognised shape types", __FILE__, __LINE__);

   return it->second(obj1, pos1, obj2, pos2);
}


}
}
