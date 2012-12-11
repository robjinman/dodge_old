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
// Range::dbg_render
//===========================================
void Range::dbg_render(const Colour& fillColour, const Colour& lineColour, Renderer::int_t lineWidth, int z) const {
   m_z = z;

   m_quad.setLineWidth(lineWidth);
   m_quad.setFillColour(fillColour);
   m_quad.setLineColour(lineColour);
   m_quad.setRenderTransform(m_pos.x, m_pos.y, m_z);

   m_quad.render();
}

//===========================================
// Range::dbg_unrender
//===========================================
void Range::dbg_unrender() const {
   m_quad.unrender();
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
