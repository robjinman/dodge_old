/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <Range.hpp>
#ifdef DEBUG
   #include <math/primitives/Quad.hpp>
#endif


using namespace std;


namespace Dodge {


#ifdef DEBUG
Graphics2d Range::m_graphics2d = Graphics2d();
#endif


//===========================================
// Range::Range
//===========================================
Range::Range(const XmlNode data) {
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
void Range::dbg_draw(int z) const {
   Quad quad(m_size);
   m_graphics2d.drawPrimitive(quad, m_pos.x, m_pos.y, z);
}
#endif

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
   return range.m_pos.x > m_pos.x && range.m_pos.x < m_pos.x + m_size.x
      && range.m_pos.x + range.m_size.x < m_pos.x + m_size.x
      && range.m_pos.y > m_pos.y && range.m_pos.y < m_pos.y + m_size.y
      && range.m_pos.y + range.m_size.y < m_pos.y + m_size.y;
}


}
