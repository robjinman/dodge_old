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
      Box2dPhysics(Entity* entity, const XmlNode data);

      Box2dPhysics(const Box2dPhysics& copy, Entity* entity)
         : EntityPhysics(copy, entity),
           m_entity(entity),
           m_body(NULL),
           m_opts(copy.m_opts) {}

      Box2dPhysics(Entity* entity)
         : EntityPhysics(entity),
           m_entity(entity),
           m_body(NULL),
           m_opts(false, false, 1.f, 0.3f) {}

      Box2dPhysics(Entity* entity, const EntityPhysics::options_t& options)
         : EntityPhysics(entity, options),
           m_entity(entity),
           m_body(NULL),
           m_opts(options) {}

      virtual void assignData(const XmlNode data);
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
      void primitiveToBox2dBody(const Primitive& shape, const EntityPhysics::options_t& opts, b2Body* body, uint_t* nFixtures) const;
      void updatePos(EEvent* event);
      void deepCopy(const Box2dPhysics& copy);

      static void updateFrameRate();
      static void entityMovedHandler(EEvent* ev);

      static EventManager m_eventManager;
      static std::set<EEvent*> m_ignore;

      static float32_t m_frameRate;
      static float32_t m_worldUnitsPerMetre;
      static int m_v_iterations;
      static int m_p_iterations;

      static std::map<Entity*, Box2dPhysics*> m_physEnts;
      static b2World m_world;
      static b2Vec2 m_gravity;

      bool m_init;

      Entity* m_entity;
      b2Body* m_body;
      uint_t m_numFixtures;

      options_t m_opts;
};


}


#endif
