/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2012
 */

#include <cstring>
#include <sstream>
#include <Poly.hpp>


using namespace std;
using namespace rapidxml;


namespace Dodge {


#ifdef DEBUG
//===========================================
// Poly::dbg_print
//===========================================
void Poly::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "Poly\n";

   for (int v = 0; v < m_nVerts; ++v) {
      for (int i = 0; i < tab + 1; ++i) out << "\t";
      out << "vert " << v << ": (" << m_verts[v].x << ", " << m_verts[v].y << ")\n";
   }
}
#endif

//===========================================
// Poly::Poly
//===========================================
Poly::Poly() : m_nVerts(0) {}

//===========================================
// Poly::Poly
//===========================================
Poly::Poly(const Poly& poly) {
   for (int i = 0; i < poly.m_nVerts; ++i)
      m_verts[i] = poly.m_verts[i];

   m_nVerts = poly.m_nVerts;
}

//===========================================
// Poly::assignData
//===========================================
void Poly::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Poly") != 0)
      throw Exception("Error parsing XML for instance of class Poly", __FILE__, __LINE__);

   m_nVerts = 0;

   xml_node<>* node = data->first_node();
   while (node) {
      if (strcmp(node->name(), "Vec2f") == 0) {
         Vec2f vert;
         vert.assignData(node);

         m_verts[m_nVerts] = vert;

         ++m_nVerts;
      }
      node = node->next_sibling();
   }
}

//===========================================
// Poly::addVertex
//===========================================
void Poly::addVertex(const Vec2f& vert) {
   if (m_nVerts >= MAX_VERTS) {
      stringstream msg;
      msg << "Error adding vertex; MAX_VERTS = " << MAX_VERTS;
      throw Exception(msg.str(), __FILE__, __LINE__);
   }

   m_verts[m_nVerts] = vert;
   ++m_nVerts;
}

//===========================================
// Poly::removeVertex
//===========================================
void Poly::removeVertex(int idx) {
   if (idx > m_nVerts - 1 || idx < 0)
      throw Exception("Index out of range", __FILE__, __LINE__);

   for (int i = idx; i < m_nVerts - 1; ++i)
      m_verts[i] = m_verts[i + 1];

   --m_nVerts;
}

//===========================================
// Poly::insertVertex
//===========================================
void Poly::insertVertex(int idx, const Vec2f& vert) {
   if (m_nVerts >= MAX_VERTS) {
      stringstream msg;
      msg << "Error adding vertex; MAX_VERTS = " << MAX_VERTS;
      throw Exception(msg.str(), __FILE__, __LINE__);
   }

   if (idx > m_nVerts - 1 || idx < 0)
      throw Exception("Index out of range", __FILE__, __LINE__);

   ++m_nVerts;

   for (int i = m_nVerts - 1; i > idx; --i)
      m_verts[i] = m_verts[i - 1];

   m_verts[idx] = vert;
}

//===========================================
// Poly::rotate
//===========================================
void Poly::rotate(double rads, const Vec2f& p) {
   for (int i = 0; i < m_nVerts; ++i)
      m_verts[i].rotate(p, rads);
}

//===========================================
// Poly::scaleFromZero
//
//! @brief Scale the distance from each vertex to (0, 0) by sv.
//===========================================
void Poly::scale(const Vec2f& sv) {
   if (sv.x == 1.0 && sv.y == 1.0) return;

   for (int i = 0; i < m_nVerts; ++i) {
      m_verts[i].x = m_verts[i].x * sv.x;
      m_verts[i].y = m_verts[i].y * sv.y;
   }
}

//===========================================
// Poly::computeMinimum
//
//! @brief Returns lowest corner of polygon's bounding rectangle
//===========================================
Vec2f Poly::computeMinimum() const {
   if (m_nVerts == 0) return Vec2f(0, 0);

   Vec2f min(m_verts[0].x, m_verts[0].y);

   for (int i = 1; i < m_nVerts; ++i) {
      if (m_verts[i].x < min.x) min.x = m_verts[i].x;
      if (m_verts[i].y < min.y) min.y = m_verts[i].y;
   }

   return min;
}

//===========================================
// Poly::computeMaximum
//
//! @brief Returns highest corner of polygon's bounding rectangle
//===========================================
Vec2f Poly::computeMaximum() const {
   if (m_nVerts == 0) return Vec2f(0, 0);

   Vec2f max(m_verts[0].x, m_verts[0].y);

   for (int i = 1; i < m_nVerts; ++i) {
      if (m_verts[i].x > max.x) max.x = m_verts[i].x;
      if (m_verts[i].y > max.y) max.y = m_verts[i].y;
   }

   return max;
}

//===========================================
// Poly::operator==
//===========================================
bool Poly::operator==(const Poly& rhs) const {
   if (m_nVerts != rhs.m_nVerts) return false;

   for (int i = 0; i < m_nVerts; ++i)
      if (m_verts[i] != rhs.m_verts[i]) return false;

   return true;
}


}
