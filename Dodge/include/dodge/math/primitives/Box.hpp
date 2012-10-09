/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __BOX_HPP__
#define __BOX_HPP__


#include "../../Renderer.hpp"
#include "Primitive.hpp"


namespace Dodge {


class Box : public Primitive {
   public:
      Box(float32_t w, float32_t h) : m_size(w, h) {}
      Box(const Vec2f& size) : m_size(size) {}

      virtual long typeId() const;
      virtual Primitive* clone() const;

      virtual Vec2f getMinimum() const;
      virtual Vec2f getMaximum() const;
      virtual void rotate(double rads, const Vec2f& pivot);
      virtual void scale(const Vec2f& sv);

      virtual void assignData(const rapidxml::xml_node<>* data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab) const;
#endif

      virtual void draw(float32_t x, float32_t y, int z, float32_t angle = 0.f,
         const Vec2f& pivot = Vec2f(0.f, 0.f)) const;

      inline const Vec2f& getDimensions() const;
      inline float32_t getWidth() const;
      inline float32_t getHeight() const;
      inline void setDimensions(float32_t w, float32_t h);
      inline void setWidth(float32_t w);
      inline void setHeight(float32_t h);

      inline bool operator==(const Box& rhs) const;
      inline bool operator!=(const Box& rhs) const;

      virtual ~Box() {}

   private:
      void drawSolid(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const;
      void drawHollow(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const;

      static Renderer m_renderer;

      Vec2f m_size;
};

//===========================================
// Box::operator==
//===========================================
inline bool Box::operator==(const Box& rhs) const {
   return m_size == rhs.m_size;
}

//===========================================
// Box::operator!=
//===========================================
inline bool Box::operator!=(const Box& rhs) const {
   return !(*this == rhs);
}

//===========================================
// Box::getDimensions
//===========================================
inline const Vec2f& Box::getDimensions() const {
   return m_size;
}

//===========================================
// Box::getWidth
//===========================================
inline float32_t Box::getWidth() const {
   return m_size.x;
}

//===========================================
// Box::getHeight
//===========================================
inline float32_t Box::getHeight() const {
   return m_size.y;
}

//===========================================
// Box::setDimensions
//===========================================
inline void Box::setDimensions(float32_t w, float32_t h) {
   m_size.x = w;
   m_size.y = h;
}

//===========================================
// Box::setWidth
//===========================================
inline void Box::setWidth(float32_t w) {
   m_size.x = w;
}

//===========================================
// Box::setHeight
//===========================================
inline void Box::setHeight(float32_t h) {
   m_size.y = h;
}


}


#endif
