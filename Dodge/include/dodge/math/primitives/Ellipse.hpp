#ifndef __ELLIPSE_HPP__
#define __ELLIPSE_HPP__


#include "Primitive.hpp"


namespace Dodge {


class Ellipse : public Primitive {
   public:
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
