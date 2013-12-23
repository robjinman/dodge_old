/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __BOX_2D_PHYSICS_HPP__
#define __BOX_2D_PHYSICS_HPP__


#include "../Box2D/Box2D.h"
#include "definitions.hpp"
#include "EntityPhysics.hpp"
#include "Box2dContactListener.hpp"


namespace Dodge {


class Entity;
class EEvent;
class EEntityMoved;

class Box2dPhysics : public EntityPhysics {
   public:
      Box2dPhysics(Entity* entity, const XmlNode data);
      Box2dPhysics(const Box2dPhysics& copy, Entity* entity);
      Box2dPhysics(Entity* entity);
      Box2dPhysics(Entity* entity, const EntityPhysics::options_t& options);

      virtual size_t getSize() const;
      virtual void assignData(const XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void update();
      virtual void onEvent(const EEvent* event);

      virtual void applyLinearImpulse(const Vec2f& impulse, const Vec2f& p);
      virtual void applyForce(const Vec2f& force, const Vec2f& p);
      virtual void applyLinearImpulse(const Vec2f& impulse);
      virtual void applyForce(const Vec2f& force);
      virtual void makeDynamic();
      virtual void makeStatic();

      virtual void setLinearVelocity(const Vec2f& v);
      virtual Vec2f getLinearVelocity() const;

      static void loadSettings(const std::string& file);
      static void step();

      virtual ~Box2dPhysics() {}

   private:
      void setEntity(Entity* entity);

      void constructBody();
      void shapeToBox2dBody(const Shape& shape, const EntityPhysics::options_t& opts, b2Body* body, uint_t* nFixtures) const;
      void updatePos(const EEvent* event);
      void deepCopy(const Box2dPhysics& copy);

      static void init();
      static void entityMovedHandler(EEvent* ev);

      static EventManager m_eventManager;
      static std::set<long> m_ignore;

      static float32_t m_timeStep;
      static float32_t m_worldUnitsPerMetre;
      static int m_v_iterations;
      static int m_p_iterations;

      static b2World m_world;
      static b2Vec2 m_gravity;

      static Box2dContactListener m_contactListener;
      static bool m_isInitialised;

      bool m_init;

      Entity* m_entity;
      b2Body* m_body;
      uint_t m_numFixtures;

      options_t m_opts;
};


}


#endif
