#ifndef __SHAPE_HPP__
#define __SHAPE_HPP__


#include <memory>
#include <boost/shared_ptr.hpp>
#include "Entity.hpp"
#include "math/primitives/Primitive.hpp"
#include "renderer/Renderer.hpp"
#include "xml/xml.hpp"


namespace Dodge {


class Shape : public Entity {
   public:
      explicit Shape(const XmlNode data);
      Shape(long type, std::unique_ptr<Primitive> shape);
      Shape(long name, long type, std::unique_ptr<Primitive> shape);
      Shape(const Shape& copy);

      virtual Shape* clone() const;
      virtual void assignData(const XmlNode data);
      virtual void draw(const Vec2f& at) const;

   private:
      static Renderer m_renderer;
};

typedef boost::shared_ptr<Shape> pShape_t;


}


#endif
