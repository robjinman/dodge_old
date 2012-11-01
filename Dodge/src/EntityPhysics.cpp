/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <EntityPhysics.hpp>


namespace Dodge {


//===========================================
// EntityPhysics::EntityPhysics
//===========================================
EntityPhysics::EntityPhysics(Entity* entity, pEntityPhysicsImpl_t impl)
   : m_impl(std::move(impl)) {

   m_impl->setEntity(entity);
}

//===========================================
// EntityPhysics::EntityPhysics
//===========================================
EntityPhysics::EntityPhysics(const EntityPhysics& copy, Entity* entity)
   : m_impl(m_impl->clone()) {

   m_impl->setEntity(entity);
}

//===========================================
// EntityPhysics::addToWorld
//===========================================
void EntityPhysics::addToWorld() {
   m_impl->addToWorld();
}

//===========================================
// EntityPhysics::removeFromWorld
//===========================================
void EntityPhysics::removeFromWorld() {
   m_impl->removeFromWorld();
}

//===========================================
// EntityPhysics::assignData
//===========================================
void EntityPhysics::assignData(const rapidxml::xml_node<>* data) {
   m_impl->assignData(data);
}

#ifdef DEBUG
//===========================================
// EntityPhysics::dbg_print
//===========================================
void EntityPhysics::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "EntityPhysics:\n";
   m_impl->dbg_print(out, tab + 1);
}
#endif


}
