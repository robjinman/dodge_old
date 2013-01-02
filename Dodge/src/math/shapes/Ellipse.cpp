/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <math/shapes/Ellipse.hpp>
#include <StringId.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Ellipse::Ellipse
//===========================================
Ellipse::Ellipse(const XmlNode data)
   : Asset(internString("Ellipse")) {

   // TODO
}

//===========================================
// Ellipse::Ellipse
//===========================================
Ellipse::Ellipse(const Ellipse& copy)
   : Asset(internString("Ellipse")) {

   // TODO
}

//===========================================
// Ellipse::typeId
//===========================================
long Ellipse::typeId() const {
   static long typeId = internString("Ellipse");

   return typeId;
}

//===========================================
// Ellipse::getSize
//===========================================
size_t Ellipse::getSize() const {
   return sizeof(Ellipse); // TODO
}

//===========================================
// Ellipse::clone
//===========================================
Ellipse* Ellipse::clone() const {
   return new Ellipse(*this);
}

#ifdef DEBUG
//===========================================
// Ellipse::dbg_print
//===========================================
void Ellipse::dbg_print(std::ostream& out, int tab) const {
   // TODO
}
#endif

//===========================================
// Ellipse::getMinimum
//===========================================
Vec2f Ellipse::getMinimum() const {
   // TODO
   return Vec2f(0.f, 0.f);
}

//===========================================
// Ellipse::getMaximum
//===========================================
Vec2f Ellipse::getMaximum() const {
   // TODO
   return Vec2f(0.f, 0.f);
}

//===========================================
// Ellipse::rotate
//===========================================
void Ellipse::rotate(float32_t rads, const Vec2f& pivot) {
   // TODO
}

//===========================================
// Ellipse::scale
//===========================================
void Ellipse::scale(const Vec2f& sv) {
   // TODO
}


}
