/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __RANGE_HPP__
#define __RANGE_HPP__


#include "xml/xml.hpp"
#include "math/Vec2f.hpp"
#ifdef DEBUG
   #include <ostream>
   #include "math/shapes/Quad.hpp"
#endif


namespace Dodge {


class Range {
   public:
      Range()
         : m_pos(0.f, 0.f),
           m_size(0.f, 0.f)
#ifdef DEBUG
         , m_quad(m_size)
#endif
           {}

      explicit Range(const XmlNode data);

      Range(float32_t x, float32_t y, float32_t w, float32_t h)
         : m_pos(x, y),
           m_size(w, h)
#ifdef DEBUG
         , m_quad(m_size)
#endif
           {}

      Range(const Vec2f& pos, const Vec2f& size)
         : m_pos(pos),
           m_size(size)
#ifdef DEBUG
         , m_quad(m_size)
#endif
           {}

#ifdef DEBUG
      void dbg_print(std::ostream& out, int tab) const;

      void dbg_render(const Colour& fillColour, const Colour& lineColour, Renderer::int_t lineWidth, int z) const;
      void dbg_unrender() const;
#endif

      inline void setPosition(float32_t x, float32_t y);
      inline void setSize(float32_t w, float32_t h);
      inline void setPosition(const Vec2f& pos);
      inline void setSize(const Vec2f& size);
      inline const Vec2f& getPosition() const;
      inline const Vec2f& getSize() const;

      bool overlaps(const Range& range) const;
      bool contains(const Range& range) const;
      inline bool contains(const Vec2f& p) const;

   private:
      Vec2f m_pos;
      Vec2f m_size;

#ifdef DEBUG
      Quad m_quad;
      mutable int m_z;
#endif
};

//===========================================
// Range::setPosition
//===========================================
inline void Range::setPosition(float32_t x, float32_t y) {
   m_pos.x = x;
   m_pos.y = y;

#ifdef DEBUG
   m_quad.setRenderTransform(m_pos.x, m_pos.y, m_z);
#endif
}

//===========================================
// Range::setSize
//===========================================
inline void Range::setSize(float32_t w, float32_t h) {
   m_size.x = w;
   m_size.y = h;

#ifdef DEBUG
   m_quad.setVertex(1, Vec2f(m_size.x, 0.f));
   m_quad.setVertex(2, Vec2f(m_size.x, m_size.y));
#endif
}

//===========================================
// Range::setPosition
//===========================================
inline void Range::setPosition(const Vec2f& pos) {
   setPosition(pos.x, pos.y);
}

//===========================================
// Range::setSize
//===========================================
inline void Range::setSize(const Vec2f& size) {
   setSize(size.x, size.y);
}

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

//===========================================
// Range::contains
//===========================================
inline bool Range::contains(const Vec2f& p) const {
   return p.x > m_pos.x && p.x < m_pos.x + m_size.x
      && p.y > m_pos.y && p.y < m_pos.y + m_size.y;
}


}


#endif
