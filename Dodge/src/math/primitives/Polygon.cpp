#include <sstream>
#include <cstring>
#include <definitions.hpp>
#include <math/primitives/Polygon.hpp>
#include <StringId.hpp>
#include <cml/cml.h>


using namespace cml;
using namespace rapidxml;
using namespace std;


namespace Dodge {


Renderer Polygon::m_renderer = Renderer();


//===========================================
// Polygon::Polygon
//===========================================
Polygon::Polygon() : m_nVerts(0) {
   m_verts.resize(Polygon::MAX_VERTS);
}

//===========================================
// Polygon::Polygon
//
// Contruct deep copy
//===========================================
Polygon::Polygon(const Polygon& poly) {
   for (int i = 0; i < poly.m_nVerts; ++i) {
      boost::shared_ptr<Vec2f> vert(new Vec2f(*poly.m_verts[i])); // Make copy of vertex
      m_verts.push_back(vert);
   }

   m_nVerts = poly.m_nVerts;

   restructure();
}

//===========================================
// Polygon::clone
//===========================================
Primitive* Polygon::clone() const {
   return new Polygon(*this);
}

#ifdef DEBUG
//===========================================
// Poly::dbg_print
//===========================================
void Polygon::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "Polygon\n";

   for (int v = 0; v < m_nVerts; ++v) {
      for (int i = 0; i < tab + 1; ++i) out << "\t";
      out << "vert " << v << ": (" << m_verts[v]->x << ", " << m_verts[v]->y << ")\n";
   }
}
#endif

//===========================================
// Polygon::typeId
//===========================================
long Polygon::typeId() const {
   static long typeId = internString("Polygon");

   return typeId;
}

//===========================================
// Polygon::assignData
//===========================================
void Polygon::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Poly") != 0)
      throw Exception("Error parsing XML for instance of class Poly", __FILE__, __LINE__);

   clear();

   xml_node<>* node = data->first_node();
   while (node) {
      if (strcmp(node->name(), "Vec2f") == 0) {
         boost::shared_ptr<Vec2f> vert(new Vec2f);
         vert->assignData(node);

         m_verts[m_nVerts] = vert;

         ++m_nVerts;
      }
      node = node->next_sibling();
   }

   restructure();
}

//===========================================
// Polygon::isConvex
//===========================================
bool Polygon::isConvex() const {
   // TODO
   return true;
}

//===========================================
// Polygon::subdivide
//
// Divide polygon into convex children
//===========================================
void Polygon::subdivide() {
   // TODO
}

//===========================================
// Polygon::absorbChildren
//
// If a polygon becomes convex again, it should re-absorb its children
//===========================================
void Polygon::absorbChildren() {
   // TODO
}

//===========================================
// Polygon::restructure
//
// Test for convexity, and subdivide if necessary
//===========================================
void Polygon::restructure() {
   if (m_children.size() == 0) {
      if (!isConvex()) subdivide(); // We've found a non-convex leaf
   }
   else {
      for (uint_t i = 0; i < m_children.size(); ++i)
         m_children[i].restructure();

      if (isConvex()) absorbChildren();
   }
}

//===========================================
// Polygon::addVertex
//===========================================
void Polygon::addVertex(const Vec2f& vert) {
   if (m_nVerts >= MAX_VERTS) {
      stringstream msg;
      msg << "Error adding vertex; MAX_VERTS = " << MAX_VERTS;
      throw Exception(msg.str(), __FILE__, __LINE__);
   }

   m_verts[m_nVerts] = boost::shared_ptr<Vec2f>(new Vec2f(vert));
   ++m_nVerts;

   restructure();
}

//===========================================
// Polygon::removeVertex
//===========================================
void Polygon::removeVertex(int idx) {
   if (idx > m_nVerts - 1 || idx < 0)
      throw Exception("Index out of range", __FILE__, __LINE__);

   for (int i = idx; i < m_nVerts - 1; ++i)
      m_verts[i] = m_verts[i + 1];

   --m_nVerts;

   restructure();
}

//===========================================
// Polygon::insertVertex
//===========================================
void Polygon::insertVertex(int idx, const Vec2f& vert) {
   if (idx > m_nVerts - 1 || idx < 0)
      throw Exception("Index out of range", __FILE__, __LINE__);

   ++m_nVerts;

   for (int i = m_nVerts - 1; i > idx; --i)
      m_verts[i] = m_verts[i - 1];

   *m_verts[idx] = vert;

   restructure();
}

//===========================================
// Polygon::drawHollow
//===========================================
void Polygon::drawHollow(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const {
   m_renderer.setMode(Renderer::NONTEXTURED_ALPHA);

   int n = m_nVerts;

   matrix44f_c rotation;
   matrix44f_c translation;
   matrix44f_c modelView;

   float32_t rads = DEG_TO_RAD(angle);
   matrix_rotation_euler(rotation, 0.f, 0.f, rads, euler_order_xyz);
   matrix_translation(translation, x, y, 0.f);
   modelView = translation * rotation;

   m_renderer.setMatrix(modelView.data());

   Renderer::vertexElement_t verts[6];

   for (int i = 0; i < n; ++i) {
      const Vec2f& p1 = getVertex(i);
      const Vec2f& p2 = getVertex((i + 1) % n);

      verts[0] = p1.x;
      verts[1] = p1.y;
      verts[2] = static_cast<float32_t>(z);
      verts[3] = p2.x;
      verts[4] = p2.y;
      verts[5] = static_cast<float32_t>(z);

      m_renderer.setGeometry(verts, Renderer::LINES, 2);
      m_renderer.render();
   }
}

//===========================================
// Polygon::drawSolid
//===========================================
void Polygon::drawSolid(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const {
   if (m_children.size() == 0) {
      m_renderer.setMode(Renderer::NONTEXTURED_ALPHA);
      int n = m_nVerts;

      float32_t fZ = static_cast<float32_t>(z);


      // Construct triangle fan

      // The number of vertices in a triangle fan is 3n-6,
      // where n is the number of vertices in the polygon.
      Renderer::vertexElement_t verts[3 * 3 * MAX_VERTS - 6];

      int i = 0;
      for (int v = 1; v < n - 1; ++v) {
         verts[3 * i + 0] = getVertex(0).x;
         verts[3 * i + 1] = getVertex(0).y;
         verts[3 * i + 2] = fZ;
         i++;

         verts[3 * i + 0] = getVertex(v).x;
         verts[3 * i + 1] = getVertex(v).y;
         verts[3 * i + 2] = fZ;
         i++;

         verts[3 * i + 0] = getVertex(v + 1).x;
         verts[3 * i + 1] = getVertex(v + 1).y;
         verts[3 * i + 2] = fZ;
         i++;
      }

      matrix44f_c rotation;
      matrix44f_c translation;
      matrix44f_c modelView;

      float32_t rads = DEG_TO_RAD(angle);
      matrix_rotation_euler(rotation, 0.f, 0.f, rads, euler_order_xyz);
      matrix_translation(translation, x, y, 0.f);
      modelView = translation * rotation;

      m_renderer.setMatrix(modelView.data());
      m_renderer.setGeometry(verts, Renderer::TRIANGLES, 3 * n - 6);
      m_renderer.render();
   }
   else {
      for (uint_t i = 0; i < m_children.size(); ++i)
         m_children[i].draw(x, y, z, angle, pivot);
   }
}

//===========================================
// Polygon::draw
//===========================================
void Polygon::draw(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const {
   const Renderer::Brush& brush = m_renderer.getBrush();

   if (brush.getFillColour()[3] != 0.f)
      drawSolid(x, y, z, angle, pivot);

   if (brush.getLineColour()[3] != 0.f && brush.getLineWidth() > 0)
      drawHollow(x, y, z, angle, pivot);
}

//===========================================
// Polygon::getMinimum
//===========================================
Vec2f Polygon::getMinimum() const {
   if (m_nVerts == 0) return Vec2f(0, 0);

   Vec2f min(m_verts[0]->x, m_verts[0]->y);

   for (int i = 1; i < m_nVerts; ++i) {
      if (m_verts[i]->x < min.x) min.x = m_verts[i]->x;
      if (m_verts[i]->y < min.y) min.y = m_verts[i]->y;
   }

   return min;
}

//===========================================
// Polygon::getMaximum
//===========================================
Vec2f Polygon::getMaximum() const {
   if (m_nVerts == 0) return Vec2f(0, 0);

   Vec2f max(m_verts[0]->x, m_verts[0]->y);

   for (int i = 1; i < m_nVerts; ++i) {
      if (m_verts[i]->x > max.x) max.x = m_verts[i]->x;
      if (m_verts[i]->y > max.y) max.y = m_verts[i]->y;
   }

   return max;
}

//===========================================
// Polygon::rotate
//===========================================
void Polygon::rotate(double deg, const Vec2f& p) {
   for (int i = 0; i < m_nVerts; ++i)
      m_verts[i]->rotate(p, deg);
}

//===========================================
// Polygon::scale
//===========================================
void Polygon::scale(const Vec2f& sv) {
   if (sv.x == 1.0 && sv.y == 1.0) return;

   for (int i = 0; i < m_nVerts; ++i) {
      m_verts[i]->x = m_verts[i]->x * sv.x;
      m_verts[i]->y = m_verts[i]->y * sv.y;
   }
}

//===========================================
// Polygon::operator==
//===========================================
bool Polygon::operator==(const Polygon& rhs) const {
   if (m_nVerts != rhs.m_nVerts) return false;

   for (int i = 0; i < m_nVerts; ++i)
      if (*m_verts[i] != *rhs.m_verts[i]) return false;

   return true;
}

//===========================================
// Polygon::operator!=
//===========================================
bool Polygon::operator!=(const Polygon& rhs) const {
   return !(*this == rhs);
}


}
