/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include "Shape.hpp"


namespace Dodge {


Renderer Shape::m_renderer = Renderer();


//===========================================
// Shape::Shape
//===========================================
Shape::Shape(long type, std::unique_ptr<Primitive> shape)
   : Entity(type) {

   // Set to silent to prevent an EEntityMoved event being propagated -- because EEntityMoved
   // objects need a boost::shared_ptr<Entity>, which cannot be retrieved via Entity::getSharedPtr()
   // until entity is fully constructed (otherwise boost::bad_weak_ptr exception will occur).
   bool s = isSilent();
   setSilent(true);

   setShape(std::move(shape));

   setSilent(s);
}

//===========================================
// Shape::Shape
//===========================================
Shape::Shape(long name, long type, std::unique_ptr<Primitive> shape)
   : Entity(name, type) {

   bool s = isSilent();
   setSilent(true);

   setShape(std::move(shape));

   setSilent(s);
}

//===========================================
// Shape::draw
//===========================================
void Shape::draw(const Vec2f& at) const {
   m_renderer.attachBrush(getRenderBrush());
   getShape().draw(getTranslation().x + at.x, getTranslation().y + at.y, getZ());
}


}
