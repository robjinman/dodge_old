/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ENTITY_PHYSICS_HPP__
#define __ENTITY_PHYSICS_HPP__


#include <map>
#include <set>
#include "../Box2D/Box2D.h"
#include "Entity.hpp"
#include "EventManager.hpp"
#include "Exception.hpp"


namespace Dodge {


class EEntityMoved;

class EntityPhysics {
   public:
      class options_t {
         public:
            bool dynamic;
            bool fixedAngle;
            float density;
            float friction;

            options_t(const options_t& c)
               : dynamic(c.dynamic), fixedAngle(c.fixedAngle), density(c.density), friction(c.friction) {}

            options_t()
               : dynamic(false), fixedAngle(false), density(1.0), friction(0.3) {}

            options_t(bool dyn, bool fix, float den, float fri)
               : dynamic(dyn), fixedAngle(fix), density(den), friction(fri) {}
      };

      EntityPhysics(Entity* entity, const options_t& opts)
         : m_init(false), m_body(NULL), m_opts(opts), m_entity(entity), m_numFixtures(0) {}

      static void loadSettings(const std::string& file);
      static void update();

      static void entityMovedHandler(EEvent* ev);

      void applyLinearImpulse(const Vec2f& impulse, const Vec2f& p);
      void applyForce(const Vec2f& force, const Vec2f& p);
      inline void applyLinearImpulse(const Vec2f& impulse);
      inline void applyForce(const Vec2f& force);
      void makeDynamic();
      void makeStatic();
      Vec2f getLinearVelocity() const;

      virtual ~EntityPhysics() {}

   protected:
      virtual void addToWorld();
      virtual void removeFromWorld();

      const options_t& getPhysOptions() const;
      void setPhysOptions(const options_t& opts);

   private:
      void constructBody();
      void updatePos(EEntityMoved* event);
      static void updateFrameRate();

      static EventManager m_eventManager;
      static std::set<EEvent*> m_ignore;

      static float m_frameRate;
      static float m_scaledPixPerMetre;
      static int m_v_iterations;
      static int m_p_iterations;

      static std::map<Entity*, EntityPhysics*> m_physEnts;
      static b2World m_world;
      static b2Vec2 m_gravity;

      bool m_init;

      b2Body* m_body;
      options_t m_opts;
      Entity* m_entity;
      unsigned int m_numFixtures;
};

inline void EntityPhysics::applyLinearImpulse(const Vec2f& impulse) {
   if (!m_init) return;
   applyLinearImpulse(impulse, Vec2f(m_body->GetWorldCenter().x * m_scaledPixPerMetre, m_body->GetWorldCenter().y * m_scaledPixPerMetre));
}

inline void EntityPhysics::applyForce(const Vec2f& force) {
   if (!m_init) return;
   applyForce(force, Vec2f(m_body->GetWorldCenter().x * m_scaledPixPerMetre, m_body->GetWorldCenter().y * m_scaledPixPerMetre));
}


}


#endif /*!__ENTITY_PHYSICS_HPP__*/
