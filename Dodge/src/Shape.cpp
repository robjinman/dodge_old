#include "Shape.hpp"


namespace Dodge {


Renderer Shape::m_renderer = Renderer();


//===========================================
// Shape::Shape
//===========================================
Shape::Shape(long type, std::unique_ptr<Primitive> shape)
   : Entity(type) {

   setShape(std::move(shape));
}

//===========================================
// Shape::Shape
//===========================================
Shape::Shape(long name, long type, std::unique_ptr<Primitive> shape)
   : Entity(name, type) {

   setShape(std::move(shape));
}

//===========================================
// Shape::draw
//===========================================
void Shape::draw(const Vec2f& at) const {
   m_renderer.attachBrush(getRenderBrush());
   getShape().draw(getTranslation().x + at.x, getTranslation().y + at.y, getZ());
}


}
