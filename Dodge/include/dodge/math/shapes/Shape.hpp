#ifndef __PRIMITIVE_HPP__
#define __PRIMITIVE_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include <boost/shared_ptr.hpp>
#include "../../xml/xml.hpp"
#include "../Vec2f.hpp"
#include "../../Asset.hpp"
#include "../../renderer/Colour.hpp"


namespace Dodge {


// A shape does not have any explicit position defined (though
// some shapes such as polygons, which are represented as sequences of vertices,
// will have an implicit position because all vertices can be moved).
class Shape : virtual public Asset {
   public:
      virtual long typeId() const = 0;

      virtual Shape* clone() const = 0;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const = 0;
#endif
      virtual Vec2f getMinimum() const = 0;
      virtual Vec2f getMaximum() const = 0;
      virtual void rotate(float32_t deg, const Vec2f& pivot = Vec2f(0.f, 0.f)) = 0;
      virtual void scale(const Vec2f& sv) = 0;

      virtual void setFillColour(const Colour& colour) = 0;
      virtual void setLineColour(const Colour& colour) = 0;
      virtual void setLineWidth(int lineWidth) = 0;

      virtual void setRenderTransform(float32_t x, float32_t y, int z) const = 0;
      virtual void render() const = 0;
      virtual void unrender() const = 0;

      virtual ~Shape() {};
};

typedef boost::shared_ptr<Shape> pShape_t;


}


#endif
