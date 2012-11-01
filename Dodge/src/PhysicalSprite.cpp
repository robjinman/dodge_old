/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <PhysicalSprite.hpp>


namespace Dodge {


//===========================================
// PhysicalSprite::PhysicalSprite
//===========================================
PhysicalSprite::PhysicalSprite(pEntityPhysicsImpl_t impl, long type, pTexture_t texture)
   : Sprite(type, texture), EntityPhysics(this, std::move(impl)) {}

//===========================================
// PhysicalSprite::PhysicalSprite
//===========================================
PhysicalSprite::PhysicalSprite(pEntityPhysicsImpl_t impl, long name, long type, pTexture_t texture)
   : Sprite(name, type, texture), EntityPhysics(this, std::move(impl)) {}

//===========================================
// PhysicalSprite::PhysicalSprite
//===========================================
PhysicalSprite::PhysicalSprite(const PhysicalSprite& copy)
   : Sprite(copy), EntityPhysics(copy, this) {}

//===========================================
// PhysicalSprite::PhysicalSprite
//===========================================
PhysicalSprite::PhysicalSprite(const PhysicalSprite& copy, long name)
   : Sprite(copy, name), EntityPhysics(copy, this) {}

//===========================================
// PhysicalSprite::assignData
//===========================================
void PhysicalSprite::assignData(const rapidxml::xml_node<>* data) {
   Sprite::assignData(data);
   EntityPhysics::assignData(data);
}

#ifdef DEBUG
//===========================================
// PhysicalSprite::dbg_print
//===========================================
void PhysicalSprite::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "PhysicalSprite:\n";
   Sprite::dbg_print(out, tab + 1);
   EntityPhysics::dbg_print(out, tab + 1);
}
#endif

//===========================================
// PhysicalSprite::addToWorld
//===========================================
void PhysicalSprite::addToWorld() {
   EntityPhysics::addToWorld();
}

//===========================================
// PhysicalSprite::removeFromWorld
//===========================================
void PhysicalSprite::removeFromWorld() {
   EntityPhysics::removeFromWorld();
}

//===========================================
// PhysicalSprite::update
//===========================================
void PhysicalSprite::update() {
   Sprite::update();
}


}
