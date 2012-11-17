#ifndef __PRIMITIVE_HPP__
#define __PRIMITIVE_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include <boost/shared_ptr.hpp>
#include "../../xml/xml.hpp"
#include "../Vec2f.hpp"
#include "../../Asset.hpp"


namespace Dodge {


// A primitive is a shape that does not have any explicit position defined -- though
// some primitives such as polygons, which are represented as sequences of vertices,
// will have an implicit position because all vertices can be moved.
class Primitive : public Asset {
   public:
      virtual long typeId() const = 0;

      virtual Primitive* clone() const = 0;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab) const = 0;
#endif
      virtual Vec2f getMinimum() const = 0;
      virtual Vec2f getMaximum() const = 0;
      virtual void rotate(float32_t deg, const Vec2f& pivot = Vec2f(0.f, 0.f)) = 0;
      virtual void scale(const Vec2f& sv) = 0;

      // Pivot is offset from (x, y), about which the rotation is applied
      virtual void draw(float32_t x, float32_t y, int z, float32_t angle = 0.f,
         const Vec2f& pivot = Vec2f(0.f, 0.f)) const = 0;

      virtual ~Primitive() {};
};

typedef boost::shared_ptr<Primitive> pPrimitive_t;


}


#endif
