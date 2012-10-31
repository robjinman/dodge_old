/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __BOX_2D_PHYSICS_HPP__
#define __BOX_2D_PHYSICS_HPP__


#include "../Box2D/Box2D.h"
#include "definitions.hpp"
#include "EntityPhysicsImpl.hpp"


namespace Dodge {


class Entity;
class EEvent;
class EEntityMoved;

class Box2dPhysics : public EntityPhysicsImpl {
   public:
      Box2dPhysics(bool dynamic, bool fixedAngle, float32_t density, float32_t friction)
         : m_dynamic(dynamic), m_fixedAngle(fixedAngle), m_density(density), m_friction(friction) {}

      Box2dPhysics(const Box2dPhysics& copy) {
         deepCopy(copy);
      }

      virtual EntityPhysicsImpl* clone() const;

      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void applyLinearImpulse(const Vec2f& impulse, const Vec2f& p);
      virtual void applyForce(const Vec2f& force, const Vec2f& p);
      virtual void applyLinearImpulse(const Vec2f& impulse);
      virtual void applyForce(const Vec2f& force);
      virtual void makeDynamic();
      virtual void makeStatic();
      virtual Vec2f getLinearVelocity() const;

      static void loadSettings(const std::string& file);
      static void update();

      virtual ~Box2dPhysics() {}

   private:
      void setEntity(Entity* entity);

      void constructBody();
      void updatePos(EEntityMoved* event);
      void deepCopy(const Box2dPhysics& copy);

      static void updateFrameRate();
      static void entityMovedHandler(EEvent* ev);

      static EventManager m_eventManager;
      static std::set<EEvent*> m_ignore;

      static float32_t m_frameRate;
      static float32_t m_scaledPixPerMetre;
      static int m_v_iterations;
      static int m_p_iterations;

      static std::map<Entity*, Box2dPhysics*> m_physEnts;
      static b2World m_world;
      static b2Vec2 m_gravity;

      bool m_init;

      b2Body* m_body;
      Entity* m_entity;
      unsigned int m_numFixtures;

      bool m_dynamic;
      bool m_fixedAngle;
      float32_t m_density;
      float32_t m_friction;
};


}


#endif
