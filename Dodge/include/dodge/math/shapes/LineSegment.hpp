/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __LINE_SEGMENT_HPP__
#define __LINE_SEGMENT_HPP__


#include "Shape.hpp"
#include "../../definitions.hpp"
#include "../Vec2f.hpp"
#include "../../renderer/Model.hpp"


namespace Dodge {


class LineSegment : public Shape {
   public:
      explicit LineSegment(const XmlNode data);
      LineSegment(float32_t p1x, float32_t p1y, float32_t p2x, float32_t p2y);
      LineSegment(const Vec2f& p1, const Vec2f& p2);
      LineSegment(const LineSegment& copy);

      inline void setPoint1(float32_t x, float32_t y);
      inline void setPoint1(const Vec2f& p1);
      inline void setPoint2(float32_t x, float32_t y);
      inline void setPoint2(const Vec2f& p2);

      inline Vec2f getPoint1() const;
      inline Vec2f getPoint2() const;

      virtual size_t getSize() const;
      virtual LineSegment* clone() const;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab) const;
#endif
      virtual Vec2f getMinimum() const;
      virtual Vec2f getMaximum() const;
      virtual void rotate(float32_t rads, const Vec2f& pivot);
      virtual void scale(const Vec2f& sv);

      virtual void setFillColour(const Colour& colour) const {}
      virtual void setLineColour(const Colour& colour) const;
      virtual void setLineWidth(int lineWidth) const;

      virtual void setRenderTransform(float32_t x, float32_t y, int z) const;
      virtual void draw() const;

      LineSegment& operator=(const LineSegment& rhs);

      inline bool operator==(const LineSegment& rhs) const;
      inline bool operator!=(const LineSegment& rhs) const;

      virtual long typeId() const;
      virtual ~LineSegment();

   private:
      mutable PlainNonTexturedAlphaModel m_model;
      Renderer& m_renderer;
};

//===========================================
// LineSegment::setPoint1
//===========================================
inline void LineSegment::setPoint1(float32_t x, float32_t y) {
   m_model.setVertex(0, vvv_t(x, y, 0.f));
}

//===========================================
// LineSegment::setPoint1
//===========================================
inline void LineSegment::setPoint1(const Vec2f& p1) {
   setPoint1(p1.x, p1.y);
}

//===========================================
// LineSegment::setPoint2
//===========================================
inline void LineSegment::setPoint2(float32_t x, float32_t y) {
   m_model.setVertex(1, vvv_t(x, y, 0.f));
}

//===========================================
// LineSegment::setPoint2
//===========================================
inline void LineSegment::setPoint2(const Vec2f& p2) {
   setPoint2(p2.x, p2.y);
}

//===========================================
// LineSegment::getPoint1
//===========================================
inline Vec2f LineSegment::getPoint1() const {
   vvv_t vert = m_model.getVertex(0);
   return Vec2f(vert.v1, vert.v2);
}

//===========================================
// LineSegment::getPoint2
//===========================================
inline Vec2f LineSegment::getPoint2() const {
   vvv_t vert = m_model.getVertex(1);
   return Vec2f(vert.v1, vert.v2);
}

//===========================================
// LineSegment::operator==
//===========================================
inline bool LineSegment::operator==(const LineSegment& rhs) const {
   return getPoint1() == rhs.getPoint1() && getPoint2() == rhs.getPoint2();
}

//===========================================
// LineSegment::operator!=
//===========================================
inline bool LineSegment::operator!=(const LineSegment& rhs) const {
   return !(*this == rhs);
}


}


#endif
