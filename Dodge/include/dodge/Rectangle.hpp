/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __RECTANGLE_HPP__
#define __RECTANGLE_HPP__


#include "rapidxml/rapidxml.hpp"
#include "Vec2f.hpp"


namespace Dodge {


class Rectangle {
   public:
      Rectangle()
         : m_pos(0.f, 0.f), m_size(0.f, 0.f) {}

      Rectangle(float32_t x, float32_t y, float32_t w, float32_t h)
         : m_pos(x, y), m_size(w, h) {}

      virtual void assignData(const rapidxml::xml_node<>* data);

      inline const Vec2f& getPosition() const;
      inline const Vec2f& getSize() const;

   private:
      Vec2f m_pos;
      Vec2f m_size;
};

//===========================================
// Rectangle::getPosition
//===========================================
inline const Vec2f& Rectangle::getPosition() const {
   return m_pos;
}

//===========================================
// Rectangle::getSize
//===========================================
inline const Vec2f& Rectangle::getSize() const {
   return m_size;
}


}


#endif
