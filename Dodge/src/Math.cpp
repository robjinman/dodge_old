/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <vector>
#include <math.h>
#include "Math.hpp"


using namespace std;


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

//===========================================
// Math::polyIntersect
//
// Returns true if polygons are overlapping (except if one polygon completely contains the other).
//===========================================
bool polyIntersect(const Vec2f& pos1, const Poly& poly1, const Vec2f& pos2, const Poly& poly2) {
   double xMin1 = pos1.x + poly1.computeMinimum().x;
   double xMax1 = pos1.x + poly1.computeMaximum().x;
   double yMin1 = pos1.y + poly1.computeMinimum().y;
   double yMax1 = pos1.y + poly1.computeMaximum().y;

   double xMin2 = pos2.x + poly2.computeMinimum().x;
   double xMax2 = pos2.x + poly2.computeMaximum().x;
   double yMin2 = pos2.y + poly2.computeMinimum().y;
   double yMax2 = pos2.y + poly2.computeMaximum().y;

   if (!(xMin1 <= xMax2 && xMax1 >= xMin2 && yMin1 <= yMax2 && yMax1 >= yMin2)) return false;

   for (int i = 0; i < poly1.getNumVertices(); ++i) {
      Vec2f l1p1 = poly1.getVertex(i) + pos1;
      Vec2f l1p2 = poly1.getVertex((i + 1) % poly1.getNumVertices()) + pos1;

      for (int j = 0; j < poly2.getNumVertices(); ++j) {
         Vec2f p;
         Vec2f l2p1 = poly2.getVertex(j) + pos2;
         Vec2f l2p2 = poly2.getVertex((j + 1) % poly2.getNumVertices()) + pos2;
         if (lineSegmentIntersect(l1p1, l1p2, l2p1, l2p2, p)) return true;
      }
   }

   return false;
}

//===========================================
// Math::compoundPolyIntersect
//
// Returns true if polygons are overlapping (except if one polygon completely contains the other).
//===========================================
bool compoundPolyIntersect(const Vec2f& pos1, const CompoundPoly& poly1, const Vec2f& pos2, const CompoundPoly& poly2) {
   for (int i = 0; i < poly1.getNumPolys(); ++i) {
      for (int j = 0; j < poly2.getNumPolys(); ++j) {
         if (polyIntersect(pos1, poly1.getPoly(i), pos2, poly2.getPoly(j)))
            return true;
      }
   }
   return false;
}

//===========================================
// Math::polyOverlap
//
// Returns true if poly1 and poly2 overlap
//===========================================
bool polyOverlap(const Vec2f& pos1, const Poly& poly1, const Vec2f& pos2, const Poly& poly2) {
   double xMin1 = pos1.x + poly1.computeMinimum().x;
   double xMax1 = pos1.x + poly1.computeMaximum().x;
   double yMin1 = pos1.y + poly1.computeMinimum().y;
   double yMax1 = pos1.y + poly1.computeMaximum().y;

   double xMin2 = pos2.x + poly2.computeMinimum().x;
   double xMax2 = pos2.x + poly2.computeMaximum().x;
   double yMin2 = pos2.y + poly2.computeMinimum().y;
   double yMax2 = pos2.y + poly2.computeMaximum().y;

   if (!(xMin1 <= xMax2 && xMax1 >= xMin2 && yMin1 <= yMax2 && yMax1 >= yMin2)) return false;

   // TODO: Use a general memory stack

   vector<Vec2f> lines;

   for (int i = 0; i < poly1.getNumVertices(); ++i) {
      Vec2f line = (poly1.getVertex(i) - poly1.getVertex((i + 1) % poly1.getNumVertices())) + pos1;

      // Normalise
      double sf = sqrt(line.x * line.x + line.y * line.y);
      line.x /= sf;
      line.y /= sf;

      lines.push_back(line);
   }

   for (int i = 0; i < poly2.getNumVertices(); ++i) {
      Vec2f line = (poly2.getVertex(i) - poly2.getVertex((i + 1) % poly2.getNumVertices())) + pos2;

      // Normalise
      double sf = sqrt(line.x * line.x + line.y * line.y);
      line.x /= sf;
      line.y /= sf;

      lines.push_back(line);
   }

   for (uint_t l = 0; l < lines.size(); l++) {
      double p1Min = 0, p1Max = 0, p2Min = 0, p2Max = 0;

      for (int i = 0; i < poly1.getNumVertices(); ++i) {

         // Projected point
         double pt = lines[l].x * (pos1.x + poly1.getVertex(i).x) + lines[l].y * (pos1.y + poly1.getVertex(i).y);

         if (i == 0) {
            p1Min = pt;
            p1Max = pt;
         }

         if (pt < p1Min) p1Min = pt;
         if (pt > p1Max) p1Max = pt;
      }

      for (int i = 0; i < poly2.getNumVertices(); ++i) {

         // Projected point
         double pt = lines[l].x * (pos2.x + poly2.getVertex(i).x) + lines[l].y * (pos2.y + poly2.getVertex(i).y);

         if (i == 0) {
            p2Min = pt;
            p2Max = pt;
         }

         if (pt < p2Min) p2Min = pt;
         if (pt > p2Max) p2Max = pt;
      }

      if (p1Max <= p2Min || p2Max <= p1Min) return false;
   }

   return true;
}

//===========================================
// Math::compoundPolyOverlap
//
// Returns true if poly1 and poly2 overlap
//===========================================
bool compoundPolyOverlap(const Vec2f& pos1, const CompoundPoly& poly1, const Vec2f& pos2, const CompoundPoly& poly2) {
   for (int i = 0; i < poly1.getNumPolys(); ++i) {
      for (int j = 0; j < poly2.getNumPolys(); ++j) {
         if (polyOverlap(pos1, poly1.getPoly(i), pos2, poly2.getPoly(j)))
            return true;
      }
   }
   return false;
}


}
}
