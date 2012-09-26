/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <Range.hpp>


namespace Dodge {


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


}
