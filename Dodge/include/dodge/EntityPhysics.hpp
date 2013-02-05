/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ENTITY_PHYSICS_HPP__
#define __ENTITY_PHYSICS_HPP__


#include "Entity.hpp"
#include "math/Vec2f.hpp"
#ifdef DEBUG
#include <ostream>
#include "xml/xml.hpp"
#endif


namespace Dodge {


class PhysicsException : public Exception {
   public:
      PhysicsException(const std::string& msg, const char* srcFile, int srcLine)
         : Exception(msg, srcFile, srcLine) {}
};


class EntityPhysics {
   public:
      struct options_t {
         options_t(bool dynamic_, bool fixedAngle_, float32_t density_, float32_t friction_)
            : dynamic(dynamic_), fixedAngle(fixedAngle_), density(density_), friction(friction_) {}

         bool dynamic;
         bool fixedAngle;
         float32_t density;
         float32_t friction;
      };

      explicit EntityPhysics(Entity* entity) {}
      EntityPhysics(Entity* entity, const options_t& options) {}
      EntityPhysics(const EntityPhysics& copy, Entity* entity) {}
      EntityPhysics(Entity* entity, const XmlNode data) {}

      virtual void addToWorld() = 0;
      virtual void removeFromWorld() = 0;

      virtual size_t getSize() const = 0;
      virtual Asset* clone() const = 0;
      virtual void assignData(const XmlNode data) = 0;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const = 0;
#endif
      virtual void applyLinearImpulse(const Vec2f& impulse, const Vec2f& p) = 0;
      virtual void applyForce(const Vec2f& force, const Vec2f& p) = 0;
      virtual void applyLinearImpulse(const Vec2f& impulse) = 0;
      virtual void applyForce(const Vec2f& force) = 0;
      virtual void makeDynamic() = 0;
      virtual void makeStatic() = 0;
      virtual Vec2f getLinearVelocity() const = 0;

//    Derived class must implement:
//    static void loadSettings(const std::string& file);
//    static void update();

      virtual ~EntityPhysics() {}

   protected:
      virtual void setEntity(Entity* entity) = 0;
};


}


#endif
