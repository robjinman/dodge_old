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
using namespace rapidxml;
using namespace std;


namespace Dodge {


Renderer LineSegment::m_renderer = Renderer();


//===========================================
// LineSegment::LineSegment
//===========================================
LineSegment::LineSegment(const XmlNode data) {
   if (data.name() != "LineSegment")
      throw XmlException("Error parsing XML for instance of class LineSegment; Expected 'LineSegment' tag", __FILE__, __LINE__);

   XmlNode node = data.firstChild();

   if (node.isNull() || node.name() != "Vec2f")
      throw XmlException("Error parsing XML for instance of class LineSegment; Expected 'Vec2f' tag", __FILE__, __LINE__);

   m_p1 = Vec2f(node);

   node = node.nextSibling();

   if (node.isNull() || node.name() != "Vec2f")
      throw XmlException("Error parsing XML for instance of class LineSegment; Expected 'Vec2f' tag", __FILE__, __LINE__);

   m_p2 = Vec2f(node);
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
   m_renderer.setMode(Renderer::NONTEXTURED_ALPHA);

   Renderer::vertexElement_t verts[] = {
      x + m_p1.x, y + m_p1.y, static_cast<float32_t>(z),
      x + m_p2.x, y + m_p2.y, static_cast<float32_t>(z)
   };

   matrix44f_c mv;
   identity_transform(mv);

   m_renderer.setMatrix(mv.data());
   m_renderer.setGeometry(verts, Renderer::LINES, 2);
   m_renderer.render();
}

//===========================================
// LineSegment::getMinimum
//===========================================
Vec2f LineSegment::getMinimum() const {
   // TODO
   return Vec2f(0.f, 0.f);
}

//===========================================
// LineSegment::getMaximum
//===========================================
Vec2f LineSegment::getMaximum() const {
   // TODO
   return Vec2f(0.f, 0.f);
}

//===========================================
// LineSegment::rotate
//===========================================
void LineSegment::rotate(double rads, const Vec2f& pivot) {
   // TODO
}

//===========================================
// LineSegment::scale
//===========================================
void LineSegment::scale(const Vec2f& sv) {
   // TODO
}


}
