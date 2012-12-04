/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cml/cml.h>
#include <Exception.hpp>
#include <math/primitives/LineSegment.hpp>
#include <StringId.hpp>
#include <renderer/Renderer.hpp>


using namespace cml;
using namespace std;


namespace Dodge {


Renderer LineSegment::m_renderer = Renderer();


//===========================================
// LineSegment::LineSegment
//===========================================
LineSegment::LineSegment(const XmlNode data)
   : m_model(Renderer::NONTEXTURED_ALPHA, false) {

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
// LineSegment::setLineColour
//===========================================
void LineSegment::setLineColour(const Colour& colour) {
   m_model.setColour(colour);
}

//===========================================
// LineSegment::setLineWidth
//===========================================
void LineSegment::setLineWidth(int lineWidth) {
   m_model.setLineWidth(lineWidth);
}

//===========================================
// LineSegment::setRenderTransform
//===========================================
void LineSegment::setRenderTransform(float32_t x, float32_t y, int z) const {
   m_model.setMatrixElement(12, x);
   m_model.setMatrixElement(13, y);
   m_model.setMatrixElement(14, static_cast<float32_t>(z));
}

//===========================================
// LineSegment::render
//===========================================
void LineSegment::render() const {
   m_renderer.stageModel(&m_model);
}

//===========================================
// LineSegment::unrender
//===========================================
void LineSegment::unrender() const {
   m_renderer.unstageModel(&m_model);
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
