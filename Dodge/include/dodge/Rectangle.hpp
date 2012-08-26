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

      Rectangle(const Vec2f& pos, const Vec2f& size)
         : m_pos(pos), m_size(size) {}

      virtual void assignData(const rapidxml::xml_node<>* data);

      inline const Vec2f& getPosition() const;
      inline const Vec2f& getSize() const;

      inline void setX(float32_t x);
      inline void setY(float32_t y);
      inline void setW(float32_t w);
      inline void setH(float32_t h);
      inline void setPosition(const Vec2f& pos);
      inline void setSize(const Vec2f& size);

      inline bool overlaps(const Rectangle& rect) const;

   private:
      Vec2f m_pos;
      Vec2f m_size;
};

//===========================================
// Rectangle::overlaps
//===========================================
inline bool Rectangle::overlaps(const Rectangle& rect) const {
   return m_pos.x + m_size.x > rect.m_pos.x && m_pos.x < rect.m_pos.x + rect.m_size.x
      && m_pos.y + m_size.y > rect.m_pos.y && m_pos.y < rect.m_pos.y + rect.m_size.y;
}

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

//===========================================
// Rectangle::setX
//===========================================
inline void Rectangle::setX(float32_t x) {
   m_pos.x = x;
}

//===========================================
// Rectangle::setY
//===========================================
inline void Rectangle::setY(float32_t y) {
   m_pos.y = y;
}

//===========================================
// Rectangle::setW
//===========================================
inline void Rectangle::setW(float32_t w) {
   m_size.x = w;
}

//===========================================
// Rectangle::setH
//===========================================
inline void Rectangle::setH(float32_t h) {
   m_size.y = h;
}

//===========================================
// Rectangle::setPosition
//===========================================
inline void Rectangle::setPosition(const Vec2f& pos) {
   m_pos = pos;
}

//===========================================
// Rectangle::setSize
//===========================================
inline void Rectangle::setSize(const Vec2f& size) {
   m_size = size;
}


}


#endif
