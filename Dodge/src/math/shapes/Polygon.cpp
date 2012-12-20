/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <sstream>
#include <cstring>
#include <definitions.hpp>
#include <math/shapes/Polygon.hpp>
#include <StringId.hpp>
#include <cml/cml.h>


using namespace cml;
using namespace std;


namespace Dodge {


//===========================================
// Polygon::Polygon
//===========================================
Polygon::Polygon()
   : m_nVerts(0),
     m_outlineModel(Renderer::LINES),
     m_interiorModel(Renderer::TRIANGLES),
     m_renderer(Renderer::getInstance()) {

//   m_verts.resize(Polygon::MAX_VERTS);
}

//===========================================
// Polygon::Polygon
//===========================================
Polygon::Polygon(const XmlNode data)
   : m_outlineModel(Renderer::LINES),
     m_interiorModel(Renderer::TRIANGLES),
     m_renderer(Renderer::getInstance()) {

   try {
      XML_NODE_CHECK(data, Polygon);

      clear();

      XmlNode node = data.firstChild();
      while (!node.isNull() && node.name() == "Vec2f") {
         boost::shared_ptr<Vec2f> vert(new Vec2f(node));
         m_verts.push_back(vert);

         ++m_nVerts;
         node = node.nextSibling();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Polygon; ");
      throw;
   }

   restructure();
   updateModels();
}

//===========================================
// Polygon::Polygon
//
// Contruct deep copy
//===========================================
Polygon::Polygon(const Polygon& poly)
   : m_outlineModel(Renderer::LINES),
     m_interiorModel(Renderer::TRIANGLES),
     m_renderer(Renderer::getInstance()) {

   deepCopy(poly);
}

//===========================================
// Polygon::deepCopy
//===========================================
void Polygon::deepCopy(const Polygon& copy) {
   m_verts.clear();
   m_nVerts = 0;

   for (int i = 0; i < copy.m_nVerts; ++i) {
      boost::shared_ptr<Vec2f> vert(new Vec2f(*copy.m_verts[i])); // Make copy of vertex
      m_verts.push_back(vert);
   }

   m_nVerts = copy.m_nVerts;

   restructure();

   m_outlineModel = copy.m_outlineModel;
   m_interiorModel = copy.m_interiorModel;
}

//===========================================
// Polygon::operator=
//===========================================
Polygon& Polygon::operator=(const Polygon& rhs) {
   deepCopy(rhs);
   return *this;
}

//===========================================
// Polygon::clone
//===========================================
Polygon* Polygon::clone() const {
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
// Polygon::isConvex
//===========================================
bool Polygon::isConvex() const {
   // TODO
   return true;
}

//===========================================
// Polygon::clear
//===========================================
void Polygon::clear() {
   m_nVerts = 0;
   m_verts.clear();
   m_children.clear();
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

   m_verts.push_back(boost::shared_ptr<Vec2f>(new Vec2f(vert)));
   ++m_nVerts;

   restructure();
   updateModels();
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
   updateModels();
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
   updateModels();
}

//===========================================
// Polygon::setFillColour
//===========================================
void Polygon::setFillColour(const Colour& colour) const {
   m_interiorModel.setColour(colour);
}

//===========================================
// Polygon::setLineColour
//===========================================
void Polygon::setLineColour(const Colour& colour) const {
   m_outlineModel.setColour(colour);
}

//===========================================
// Polygon::setLineWidth
//===========================================
void Polygon::setLineWidth(int lineWidth) const {
   m_outlineModel.setLineWidth(lineWidth);
}

//===========================================
// Polygon::setRenderTransform
//===========================================
void Polygon::setRenderTransform(float32_t x, float32_t y, int z) const {
   m_outlineModel.setMatrixElement(12, x);
   m_outlineModel.setMatrixElement(13, y);
   m_outlineModel.setMatrixElement(14, static_cast<float32_t>(z));

   m_interiorModel.setMatrixElement(12, x);
   m_interiorModel.setMatrixElement(13, y);
   m_interiorModel.setMatrixElement(14, static_cast<float32_t>(z));
}

//===========================================
// Polygon::draw
//===========================================
void Polygon::draw() const {
   m_renderer.draw(&m_interiorModel);
   m_renderer.draw(&m_outlineModel);
}

//===========================================
// Polygon::updateModels
//===========================================
void Polygon::updateModels() const {
   updateOutlineModel();
   updateInteriorModel();
}

//===========================================
// Polygon::updateOutlineModel
//===========================================
void Polygon::updateOutlineModel() const {
   vvv_t verts[m_nVerts * 2];

   int i = 0;
   for (int v = 0; v < m_nVerts; ++v) {
      verts[i] = { getVertex(v).x, getVertex(v % m_nVerts).y, 0.f };
      ++i;

      verts[i] = { getVertex((v + 1) % m_nVerts).x, getVertex((v + 1) % m_nVerts).y, 0.f };
      ++i;
   }

   m_outlineModel.setVertices(0, verts, m_nVerts * 2);
}

//===========================================
// Polygon::updateInteriorModel
//===========================================
void Polygon::updateInteriorModel() const { // TODO: for each convex child
   if (m_nVerts < 3) return;

   // Construct triangle fan

   // The number of vertices in a triangle fan is 3n-6,
   // where n is the number of vertices in the polygon.
   vvv_t verts[3 * m_nVerts - 6];

   int i = 0;
   for (int v = 1; v < m_nVerts - 1; ++v) {
      verts[i] = { getVertex(0).x, getVertex(0).y, 0.f };
      ++i;

      verts[i] = { getVertex(v).x, getVertex(v).y, 0.f };
      ++i;

      verts[i] = { getVertex(v + 1).x, getVertex(v + 1).y, 0.f };
      ++i;
   }

   m_interiorModel.setVertices(0, verts, 3 * m_nVerts - 6);
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
void Polygon::rotate(float32_t deg, const Vec2f& p) {
   for (int i = 0; i < m_nVerts; ++i)
      m_verts[i]->rotate(p, deg);

   updateModels();
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

   updateModels();
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

//===========================================
// Polygon::~Polygon
//===========================================
Polygon::~Polygon() {}


}
