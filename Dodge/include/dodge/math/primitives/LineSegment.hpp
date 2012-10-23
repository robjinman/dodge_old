/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __LINE_SEGMENT_HPP__
#define __LINE_SEGMENT_HPP__


#include "Primitive.hpp"
#include "../../definitions.hpp"
#include "../Vec2f.hpp"
#include "../../renderer/Renderer.hpp"


namespace Dodge {


class LineSegment : public Primitive {
   public:
      LineSegment(float32_t p1x, float32_t p1y, float32_t p2x, float32_t p2y)
         : m_p1(p1x, p1y), m_p2(p2x, p2y) {}

      LineSegment(const Vec2f& p1, const Vec2f& p2)
         : m_p1(p1), m_p2(p2) {}

      inline void setPoint1(float32_t x, float32_t y);
      inline void setPoint2(float32_t x, float32_t y);

      inline const Vec2f& getPoint1() const;
      inline const Vec2f& getPoint2() const;

      virtual Primitive* clone() const;
      virtual void assignData(const rapidxml::xml_node<>* data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab) const;
#endif
      virtual Vec2f getMinimum() const;
      virtual Vec2f getMaximum() const;
      virtual void rotate(double rads, const Vec2f& pivot);
      virtual void scale(const Vec2f& sv);

      virtual void draw(float32_t x, float32_t y, int z, float32_t angle = 0.f,
         const Vec2f& pivot = Vec2f(0.f, 0.f)) const;

      inline bool operator==(const LineSegment& rhs) const;
      inline bool operator!=(const LineSegment& rhs) const;

      virtual long typeId() const;
      virtual ~LineSegment() {}

   private:
      Vec2f m_p1;
      Vec2f m_p2;

      static Renderer m_renderer;
};

//===========================================
// LineSegment::setPoint1
//===========================================
inline void LineSegment::setPoint1(float32_t x, float32_t y) {
   m_p1.x = x;
   m_p1.y = y;
}

//===========================================
// LineSegment::setPoint2
//===========================================
inline void LineSegment::setPoint2(float32_t x, float32_t y) {
   m_p2.x = x;
   m_p2.y = y;
}

//===========================================
// LineSegment::getPoint1
//===========================================
inline const Vec2f& LineSegment::getPoint1() const {
   return m_p1;
}

//===========================================
// LineSegment::getPoint2
//===========================================
inline const Vec2f& LineSegment::getPoint2() const {
   return m_p2;
}

//===========================================
// LineSegment::operator==
//===========================================
inline bool LineSegment::operator==(const LineSegment& rhs) const {
   return m_p1 == rhs.m_p1 && m_p2 == rhs.m_p2;
}

//===========================================
// LineSegment::operator!=
//===========================================
inline bool LineSegment::operator!=(const LineSegment& rhs) const {
   return !(*this == rhs);
}


}


#endif