/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ENTITY_PHYSICS_IMPL_HPP__
#define __ENTITY_PHYSICS_IMPL_HPP__


#include <memory>
#include "Entity.hpp"
#include "math/Vec2f.hpp"


namespace Dodge {


class EntityPhysicsImpl {
   friend class EntityPhysics;

   public:
//    Constructor parameters:
//    bool dynamic
//    bool fixedAngle
//    float32_t density
//    float32_t friction

      virtual void addToWorld() = 0;
      virtual void removeFromWorld() = 0;

      virtual EntityPhysicsImpl* clone() const = 0;

      virtual void applyLinearImpulse(const Vec2f& impulse, const Vec2f& p) = 0;
      virtual void applyForce(const Vec2f& force, const Vec2f& p) = 0;
      virtual void applyLinearImpulse(const Vec2f& impulse) = 0;
      virtual void applyForce(const Vec2f& force) = 0;
      virtual void makeDynamic() = 0;
      virtual void makeStatic() = 0;
      virtual Vec2f getLinearVelocity() const = 0;

//    Must also implement:
//    static void loadSettings(const std::string& file);
//    static void update();

      virtual ~EntityPhysicsImpl() {}

   private:
      virtual void setEntity(Entity* entity) = 0;
};

typedef std::unique_ptr<EntityPhysicsImpl> pEntityPhysicsImpl_t;


}


#endif
