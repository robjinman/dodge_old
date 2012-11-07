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
void Sprite::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Sprite") != 0)
      throw Exception("Error parsing XML for instance of class Sprite", __FILE__, __LINE__);

   const xml_node<>* node = data->first_node();
   if (node) {
      Entity::assignData(node);
      node = node->next_sibling();
   }
   if (node) {
      EntityAnimations::assignData(node);
      node = node->next_sibling();
   }
   if (node) {
      EntityTransformations::assignData(node);
   }
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
