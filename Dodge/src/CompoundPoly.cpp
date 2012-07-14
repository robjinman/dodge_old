/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2012
 */

#include <cstring>
#include "CompoundPoly.hpp"


using namespace rapidxml;


namespace Dodge {


#ifdef DEBUG
//===========================================
// CompoundPoly::dbg_print
//===========================================
void CompoundPoly::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "CompoundPoly\n";

   for (unsigned int i = 0; i < m_polys.size(); ++i)
      m_polys[i].dbg_print(out, tab + 1);
}
#endif

//===========================================
// CompoundPoly::CompoundPoly
//===========================================
CompoundPoly::CompoundPoly() {}

//===========================================
// CompoundPoly::CompoundPoly
//===========================================
CompoundPoly::CompoundPoly(const CompoundPoly& poly) {
   m_polys = poly.m_polys;
}

//===========================================
// CompoundPoly::assignData
//===========================================
void CompoundPoly::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "CompoundPoly") != 0)
      throw Exception("Error parsing XML for instance of class CompoundPoly", __FILE__, __LINE__);

   m_polys.clear();

   xml_node<>* node = data->first_node();
   while (node) {
      if (strcmp(node->name(), "Poly") == 0) {
         Poly poly;
         poly.assignData(node);
         m_polys.push_back(poly);
      }
      node = node->next_sibling();
   }
}

//===========================================
// CompoundPoly::rotate
//===========================================
void CompoundPoly::rotate(double rads, const Vec2f& p) {
   for (unsigned int i = 0; i < m_polys.size(); ++i)
      m_polys[i].rotate(rads, p);
}

//===========================================
// CompoundPoly::scale
//===========================================
void CompoundPoly::scale(const Vec2f& sv) {
   for (unsigned int i = 0; i < m_polys.size(); ++i)
      m_polys[i].scale(sv);
}

//===========================================
// CompoundPoly::computeMinimum
//===========================================
Vec2f CompoundPoly::computeMinimum() const {
   if (m_polys.size() == 0) return Vec2f(0, 0);

   Vec2f a = m_polys[0].computeMinimum();

   Vec2f min(a.x, a.y);
   for (unsigned int i = 1; i < m_polys.size(); ++i) {
      a = m_polys[i].computeMinimum();
      if (a.x < min.x) min.x = a.x;
      if (a.y < min.y) min.y = a.y;
   }

   return min;
}

//===========================================
// CompoundPoly::computeMaximum
//===========================================
Vec2f CompoundPoly::computeMaximum() const {
   if (m_polys.size() == 0) return Vec2f(0, 0);

   Vec2f a = m_polys[0].computeMaximum();

   Vec2f max(a.x, a.y);
   for (unsigned int i = 1; i < m_polys.size(); ++i) {
      a = m_polys[i].computeMaximum();
      if (a.x < max.x) max.x = a.x;
      if (a.y < max.y) max.y = a.y;
   }

   return max;
}

//===========================================
// CompoundPoly::operator==
//===========================================
bool CompoundPoly::operator==(const CompoundPoly& rhs) const {
   if (m_polys.size() != rhs.m_polys.size()) return false;

   for (unsigned int i = 0; i < m_polys.size(); ++i)
      if (m_polys[i] != rhs.m_polys[i]) return false;

   return true;
}


}
