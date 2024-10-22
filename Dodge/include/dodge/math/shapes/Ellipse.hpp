/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ELLIPSE_HPP__
#define __ELLIPSE_HPP__


#include "Shape.hpp"


namespace Dodge {


class Ellipse : public Shape {
   public:
      explicit Ellipse(const XmlNode data);
      Ellipse(const Ellipse& copy);

      virtual long typeId() const;
      virtual size_t getSize() const;
      virtual Asset* clone() const;

      virtual Vec2f getMinimum() const;
      virtual Vec2f getMaximum() const;
      virtual void rotate(float32_t rads, const Vec2f& pivot);
      virtual void scale(const Vec2f& sv);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab) const;
#endif

      virtual void setFillColour(const Colour& colour) const {} // TODO
      virtual void setLineColour(const Colour& colour) const {}
      virtual void setLineWidth(int lineWidth) const {}

      virtual void setRenderTransform(float32_t x, float32_t y, float32_t z) const {}
      virtual void draw() const {}

      Ellipse& operator=(const Ellipse& rhs) { return *this; } // TODO

      inline bool operator==(const Ellipse& rhs) const;
      inline bool operator!=(const Ellipse& rhs) const;

      virtual ~Ellipse() {}

   private:
      // TODO
};

//===========================================
// Ellipse::operator==
//===========================================
inline bool Ellipse::operator==(const Ellipse& rhs) const {
   return false; // TODO
}

//===========================================
// Ellipse::operator!=
//===========================================
inline bool Ellipse::operator!=(const Ellipse& rhs) const {
   return !(*this == rhs);
}


}


#endif
