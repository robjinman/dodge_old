/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <Range.hpp>
#ifdef DEBUG
   #include <math/primitives/Quad.hpp>
#endif


namespace Dodge {


#ifdef DEBUG
Graphics2d Range::m_graphics2d = Graphics2d();
#endif


//===========================================
// Range::assignData
//===========================================
void Range::assignData(const rapidxml::xml_node<>* data) {
   // TODO
}

#ifdef DEBUG
//===========================================
// Range::dbg_print
//===========================================
void Range::dbg_print(std::ostream& out, int tab) const {
   // TODO
}

//===========================================
// Range::dbg_draw
//===========================================
void Range::dbg_draw(int z, const Colour& col) const {
   Quad quad(m_size);
   m_graphics2d.setFillColour(col);
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
