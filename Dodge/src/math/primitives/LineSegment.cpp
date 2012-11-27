/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cml/cml.h>
#include <Exception.hpp>
#include <math/primitives/LineSegment.hpp>
#include <StringId.hpp>


using namespace cml;
using namespace std;


namespace Dodge {


Renderer LineSegment::m_renderer = Renderer();


//===========================================
// LineSegment::LineSegment
//===========================================
LineSegment::LineSegment(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, LineSegment);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, Vec2f);
      m_p1 = Vec2f(node);

      node = node.nextSibling();
      XML_NODE_CHECK(node, Vec2f);
      m_p2 = Vec2f(node);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class LineSegment; ");
      throw;
   }
}

//===========================================
// LineSegment::typeId
//===========================================
long LineSegment::typeId() const {
   static long typeId = internString("LineSegment");

   return typeId;
}

//===========================================
// LineSegment::clone
//===========================================
LineSegment* LineSegment::clone() const {
   return new LineSegment(*this);
}

#ifdef DEBUG
//===========================================
// LineSegment::dbg_print
//===========================================
void LineSegment::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "LineSegment\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "p1: (" << m_p1.x << ", " << m_p1.y << ")\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "p2: (" << m_p2.x << ", " << m_p2.y << ")\n";
}
#endif

//===========================================
// LineSegment::draw
//===========================================
void LineSegment::draw(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const {

   Renderer::pModel_t model(new Renderer::Model(Renderer::NONTEXTURED_ALPHA, false));
   model->primitiveType = Renderer::LINES;
   model->verts = new Renderer::vvv_t[2];
   model->n = 2;

   Vec2f p1 = Vec2f(x, y) + m_p1;
   Vec2f p2 = Vec2f(x, y) + m_p2;

   p1.rotate(pivot, angle);
   p2.rotate(pivot, angle);

   Renderer::vvv_t verts[] = {
      {p1.x, p1.y, static_cast<float32_t>(z)},
      {p2.x, p2.y, static_cast<float32_t>(z)}
   };

   memcpy(model->verts, verts, 2 * sizeof(Renderer::vvv_t));

   m_renderer.stageModel(model);
}

//===========================================
// LineSegment::getMinimum
//===========================================
Vec2f LineSegment::getMinimum() const {
   return Vec2f(m_p1.x < m_p2.x ? m_p1.x : m_p2.x, m_p1.y < m_p2.y ? m_p1.y : m_p2.y);
}

//===========================================
// LineSegment::getMaximum
//===========================================
Vec2f LineSegment::getMaximum() const {
   return Vec2f(m_p1.x > m_p2.x ? m_p1.x : m_p2.x, m_p1.y > m_p2.y ? m_p1.y : m_p2.y);
}

//===========================================
// LineSegment::rotate
//===========================================
void LineSegment::rotate(float32_t rads, const Vec2f& pivot) {
   m_p1.rotate(pivot, rads);
   m_p2.rotate(pivot, rads);
}

//===========================================
// LineSegment::scale
//===========================================
void LineSegment::scale(const Vec2f& sv) {
   m_p1.x *= sv.x;
   m_p1.y *= sv.y;
   m_p2.x *= sv.x;
   m_p2.y *= sv.y;
}


}
