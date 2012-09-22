/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __RANGE_HPP__
#define __RANGE_HPP__


#include "rapidxml/rapidxml.hpp"
#include "Vec2f.hpp"


namespace Dodge {


class Range {
   public:
      Range()
         : m_pos(0.f, 0.f), m_size(0.f, 0.f) {}

      Range(float32_t x, float32_t y, float32_t w, float32_t h)
         : m_pos(x, y), m_size(w, h) {}

      virtual void assignData(const rapidxml::xml_node<>* data);

      inline const Vec2f& getPosition() const;
      inline const Vec2f& getSize() const;

      bool overlaps(const Range& range) const;

   private:
      Vec2f m_pos;
      Vec2f m_size;
};

//===========================================
// Range::getPosition
//===========================================
inline const Vec2f& Range::getPosition() const {
   return m_pos;
}

//===========================================
// Range::getSize
//===========================================
inline const Vec2f& Range::getSize() const {
   return m_size;
}


}


#endif
