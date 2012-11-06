/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <PhysicalEntity.hpp>


namespace Dodge {


//===========================================
// PhysicalEntity::clone
//===========================================
PhysicalEntity* PhysicalEntity::clone() const {
   return new PhysicalEntity(*this);
}

//===========================================
// PhysicalEntity::assignData
//===========================================
void PhysicalEntity::assignData(const rapidxml::xml_node<>* data) {
   Entity::assignData(data);
   EntityPhysics::assignData(data);
}

#ifdef DEBUG
//===========================================
// PhysicalEntity::dbg_print
//===========================================
void PhysicalEntity::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "PhysicalEntity:\n";
   Entity::dbg_print(out, tab + 1);
   EntityPhysics::dbg_print(out, tab + 1);
}
#endif

//===========================================
// PhysicalEntity::addToWorld
//===========================================
void PhysicalEntity::addToWorld() {
   EntityPhysics::addToWorld();
}

//===========================================
// PhysicalEntity::removeFromWorld
//===========================================
void PhysicalEntity::removeFromWorld() {
   EntityPhysics::removeFromWorld();
}

//===========================================
// PhysicalEntity::update
//===========================================
void PhysicalEntity::update() {
   Entity::update();
}


}
