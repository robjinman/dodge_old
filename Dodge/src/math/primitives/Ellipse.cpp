/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <math/primitives/Ellipse.hpp>
#include <StringId.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Ellipse::typeId
//===========================================
long Ellipse::typeId() const {
   static long typeId = internString("Ellipse");

   return typeId;
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
// Ellipse::draw
//===========================================
void Ellipse::draw(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const {
   // TODO
}

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
void Ellipse::rotate(double rads, const Vec2f& pivot) {
   // TODO
}

//===========================================
// Ellipse::scale
//===========================================
void Ellipse::scale(const Vec2f& sv) {
   // TODO
}

//===========================================
// Ellipse::assignData
//===========================================
void Ellipse::assignData(const rapidxml::xml_node<>* data) {
   // TODO
}


}
