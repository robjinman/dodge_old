/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <Range.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Range::Range
//===========================================
Range::Range(const XmlNode data)
#ifdef DEBUG
         : m_quad(Vec2f(0.f, 0.f))
#endif
         {

   try {
      XML_NODE_CHECK(data, Range);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, pos);
      m_pos = Vec2f(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, size);
      m_size = Vec2f(node.firstChild());
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Range; ");
      throw;
   }
}

//===========================================
// Range::Range
//===========================================
Range::Range()
   : m_pos(0.f, 0.f),
     m_size(0.f, 0.f)
#ifdef DEBUG
   , m_quad(m_size)
#endif
     {}

//===========================================
// Range::Range
//===========================================
Range::Range(float32_t x, float32_t y, float32_t w, float32_t h)
   : m_pos(x, y),
     m_size(w, h)
#ifdef DEBUG
   , m_quad(m_size)
#endif
     {}

//===========================================
// Range::Range
//===========================================
Range::Range(const Vec2f& pos, const Vec2f& size)
   : m_pos(pos),
     m_size(size)
#ifdef DEBUG
   , m_quad(m_size)
#endif
     {}

#ifdef DEBUG
//===========================================
// Range::dbg_print
//===========================================
void Range::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "Range\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "pos: (" << m_pos.x << ", " << m_pos.y << ")\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "size: (" << m_size.x << ", " << m_size.y << ")\n";
}

//===========================================
// Range::dbg_draw
//===========================================
void Range::dbg_draw(const Colour& fillColour, const Colour& lineColour, Renderer::int_t lineWidth, float32_t z) const {
   m_z = z;

   m_quad.setLineWidth(lineWidth);
   m_quad.setFillColour(fillColour);
   m_quad.setLineColour(lineColour);
   m_quad.setRenderTransform(m_pos.x, m_pos.y, m_z);

   m_quad.draw();
}
#endif

//===========================================
// Range::setPosition
//===========================================
void Range::setPosition(float32_t x, float32_t y) {
   m_pos.x = x;
   m_pos.y = y;
}

//===========================================
// Range::setSize
//===========================================
void Range::setSize(float32_t w, float32_t h) {
   m_size.x = w;
   m_size.y = h;

#ifdef DEBUG
   m_quad.setVertex(1, Vec2f(m_size.x, 0.f));
   m_quad.setVertex(2, Vec2f(m_size.x, m_size.y));
   m_quad.setVertex(3, Vec2f(0.f, m_size.y));
#endif
}

//===========================================
// Range::overlaps
//===========================================
bool Range::overlaps(const Range& range) const {
   return m_pos.x < (range.m_pos.x + range.m_size.x)
      && (m_pos.x + m_size.x) > range.m_pos.x
      && m_pos.y < (range.m_pos.y + range.m_size.y)
      && (m_pos.y + m_size.y) > range.m_pos.y;
}

//===========================================
// Range::contains
//===========================================
bool Range::contains(const Range& range) const {
   return range.m_pos.x >= m_pos.x && range.m_pos.x <= m_pos.x + m_size.x
      && range.m_pos.x + range.m_size.x <= m_pos.x + m_size.x
      && range.m_pos.y >= m_pos.y && range.m_pos.y <= m_pos.y + m_size.y
      && range.m_pos.y + range.m_size.y <= m_pos.y + m_size.y;
}


}
