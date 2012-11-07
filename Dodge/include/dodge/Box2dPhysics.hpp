/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __BOX_2D_PHYSICS_HPP__
#define __BOX_2D_PHYSICS_HPP__


#include "../Box2D/Box2D.h"
#include "definitions.hpp"
#include "EntityPhysics.hpp"


namespace Dodge {


class Entity;
class EEvent;
class EEntityMoved;

class Box2dPhysics : public EntityPhysics {
   public:
      Box2dPhysics(Entity* entity)
         : EntityPhysics(entity),
           m_entity(entity),
           m_body(NULL),
           m_dynamic(false),
           m_fixedAngle(false),
           m_density(1.f),
           m_friction(0.3f) {}

      Box2dPhysics(Entity* entity, const EntityPhysics::options_t& options)
         : EntityPhysics(entity, options),
           m_entity(entity),
           m_body(NULL),
           m_dynamic(options.dynamic),
           m_fixedAngle(options.fixedAngle),
           m_density(options.density),
           m_friction(options.friction) {}

      Box2dPhysics(Entity* entity, const rapidxml::xml_node<>* data)
         : EntityPhysics(entity, data),
           m_entity(entity),
           m_body(NULL) {} // TODO

      Box2dPhysics(const Box2dPhysics& copy, Entity* entity);

      virtual EntityPhysics* clone() const;
      virtual void assignData(const rapidxml::xml_node<>* data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

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

      Entity* m_entity;
      b2Body* m_body;
      unsigned int m_numFixtures;

      bool m_dynamic;
      bool m_fixedAngle;
      float32_t m_density;
      float32_t m_friction;
};


}


#endif
