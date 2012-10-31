/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ENTITY_PHYSICS_HPP__
#define __ENTITY_PHYSICS_HPP__


#include "EntityPhysicsImpl.hpp"
#include "Entity.hpp"


namespace Dodge {


class EntityPhysics {
   public:
      EntityPhysics(Entity* entity, pEntityPhysicsImpl_t impl)
         : m_impl(std::move(impl)) {

         m_impl->setEntity(entity);
      }

      EntityPhysics(const EntityPhysics& copy, Entity* entity)
         : m_impl(m_impl->clone()) {

         m_impl->setEntity(entity);
      }

      virtual void addToWorld() {
         m_impl->addToWorld();
      }

      virtual void removeFromWorld() {
         m_impl->removeFromWorld();
      }

      inline void applyLinearImpulse(const Vec2f& impulse, const Vec2f& p);
      inline void applyForce(const Vec2f& force, const Vec2f& p);
      inline void applyLinearImpulse(const Vec2f& impulse);
      inline void applyForce(const Vec2f& force);
      inline void makeDynamic();
      inline void makeStatic();
      inline Vec2f getLinearVelocity() const;

      virtual ~EntityPhysics() {}

   private:
      pEntityPhysicsImpl_t m_impl;
};

//===========================================
// EntityPhysics::applyLinearImpulse
//===========================================
inline void EntityPhysics::applyLinearImpulse(const Vec2f& impulse, const Vec2f& p) {
   m_impl->applyLinearImpulse(impulse, p);
}

//===========================================
// EntityPhysics::applyLinearImpulse
//===========================================
inline void EntityPhysics::applyLinearImpulse(const Vec2f& impulse) {
   m_impl->applyLinearImpulse(impulse);
}

//===========================================
// EntityPhysics::applyForce
//===========================================
inline void EntityPhysics::applyForce(const Vec2f& force, const Vec2f& p) {
   m_impl->applyForce(force, p);
}

//===========================================
// EntityPhysics::applyForce
//===========================================
inline void EntityPhysics::applyForce(const Vec2f& force) {
   m_impl->applyForce(force);
}

//===========================================
// EntityPhysics::makeDynamic
//===========================================
inline void EntityPhysics::makeDynamic() {
   m_impl->makeDynamic();
}

//===========================================
// EntityPhysics::makeStatic
//===========================================
inline void EntityPhysics::makeStatic() {
   m_impl->makeStatic();
}

//===========================================
// EntityPhysics::getLinearVelocity
//===========================================
inline Vec2f EntityPhysics::getLinearVelocity() const {
   return m_impl->getLinearVelocity();
}


}


#endif /*!__ENTITY_PHYSICS_HPP__*/
