#ifndef __PRIMITIVE_HPP__
#define __PRIMITIVE_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include "rapidxml/rapidxml.hpp"
#include "Vec2f.hpp"


namespace Dodge {


class Primitive {
   public:
      virtual long typeId() const = 0;
      virtual Primitive* clone() const = 0;
      virtual void assignData(const rapidxml::xml_node<>* data) = 0;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab) const = 0;
#endif
      virtual Vec2f getMinimum() const = 0;
      virtual Vec2f getMaximum() const = 0;
      virtual void rotate(double rads, const Vec2f& pivot) = 0;
      virtual void scale(const Vec2f& sv) = 0;

      // Pivot is offset from (x, y), about which the rotation is applied
      virtual void draw(float32_t x, float32_t y, int z, float32_t angle = 0.f,
         const Vec2f& pivot = Vec2f(0.f, 0.f)) const = 0;

      virtual ~Primitive() {};
};


}


#endif
