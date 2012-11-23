/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include "Shape.hpp"


using namespace std;


namespace Dodge {


Renderer Shape::m_renderer = Renderer();


//===========================================
// Shape::Shape
//===========================================
Shape::Shape(long type, unique_ptr<Primitive> shape)
   : Entity(type) {

   // Set to silent to prevent an EEntityMoved event being propagated -- because EEntityMoved
   // objects need a boost::shared_ptr<Entity>, which cannot be retrieved via Entity::getSharedPtr()
   // until entity is fully constructed (otherwise boost::bad_weak_ptr exception will occur).
   bool s = isSilent();
   setSilent(true);

   setShape(move(shape));

   setSilent(s);
}

//===========================================
// Shape::Shape
//===========================================
Shape::Shape(long name, long type, unique_ptr<Primitive> shape)
   : Entity(name, type) {

   bool s = isSilent();
   setSilent(true);

   setShape(move(shape));

   setSilent(s);
}

//===========================================
// Shape::Shape
//===========================================
Shape::Shape(const XmlNode data)
   : Entity(data.firstChild()) {

   XML_NODE_CHECK(data, Shape);
}

//===========================================
// Shape::Shape
//===========================================
Shape::Shape(const Shape& copy)
   : Entity(copy) {}

//===========================================
// Shape::Shape
//===========================================
Shape::Shape(const Shape& copy, long name)
   : Entity(copy, name) {}

//===========================================
// Shape::clone
//===========================================
Shape* Shape::clone() const {
   return new Shape(*this);
}

//===========================================
// Shape::assignData
//
// All tags and attributes are optional.
//===========================================
void Shape::assignData(const XmlNode data) {
   if (data.isNull() || data.name() != "Shape") return;

   try {
      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Entity") {
         Entity::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Shape; ");
      throw;
   }
}

//===========================================
// Shape::draw
//===========================================
void Shape::draw() const {
   m_renderer.attachBrush(getRenderBrush());
   getShape().draw(getTranslation().x, getTranslation().y, getZ());
}


}
