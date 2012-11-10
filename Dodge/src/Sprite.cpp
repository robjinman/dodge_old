/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <string>
#include <cstdio>
#include "Sprite.hpp"


using namespace rapidxml;
using namespace std;


namespace Dodge {


//===========================================
// Sprite::Sprite
//===========================================
Sprite::Sprite(const Sprite& copy)
   : Entity(copy), EntityAnimations(copy, this), EntityTransformations(copy, this) {

   deepCopy(copy);
}

//===========================================
// Sprite::Sprite
//===========================================
Sprite::Sprite(const Sprite& copy, long name)
   : Entity(copy, name), EntityAnimations(copy, this), EntityTransformations(copy, this) {

   deepCopy(copy);
}

//===========================================
// Sprite::deepCopy
//===========================================
void Sprite::deepCopy(const Sprite& copy) {}

//===========================================
// Sprite::clone
//===========================================
Sprite* Sprite::clone() const {
   return new Sprite(*this);
}

//===========================================
// Sprite::assignData
//===========================================
void Sprite::assignData(const XmlNode data) {
   if (data.isNull() || data.name() != "Sprite") return;

   XmlNode node = data.firstChild();

   Entity::assignData(node);
   node = node.nextSibling();

   EntityAnimations::assignData(node);
   node = node.nextSibling();

   EntityTransformations::assignData(node);
}

#ifdef DEBUG
//===========================================
// Sprite::dbg_print
//===========================================
void Sprite::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; i++) out << "\t";
   out << "Sprite\n";

   Entity::dbg_print(out, tab + 1);
   EntityAnimations::dbg_print(out, tab + 1);
   EntityTransformations::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Sprite::draw
//===========================================
void Sprite::draw() const {
   EntityAnimations::draw();
}

//===========================================
// Sprite::update
//===========================================
void Sprite::update() {
   Entity::update();
   EntityTransformations::update();
   EntityAnimations::update();
}


}
