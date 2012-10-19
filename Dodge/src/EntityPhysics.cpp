/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <EntityPhysics.hpp>
#include <EEntityMoved.hpp>
#include <Timer.hpp>
#include <KvpParser.hpp>
#include <Exception.hpp>
#include <StringId.hpp>
#include <utils/Functor.hpp>


using namespace std;


namespace Dodge {


float EntityPhysics::m_frameRate = 60.0;
float EntityPhysics::m_scaledPixPerMetre = 8.0;
int EntityPhysics::m_v_iterations = 6;
int EntityPhysics::m_p_iterations = 4;
EventManager EntityPhysics::m_eventManager = EventManager();
set<EEvent*> EntityPhysics::m_ignore = set<EEvent*>();
map<Entity*, EntityPhysics*> EntityPhysics::m_physEnts = map<Entity*, EntityPhysics*>();
b2Vec2 EntityPhysics::m_gravity = b2Vec2(0.0, -9.8);
b2World EntityPhysics::m_world = b2World(m_gravity, true);


//===========================================
// EntityPhysics::getPhysOptions
//===========================================
const EntityPhysics::options_t& EntityPhysics::getPhysOptions() const {
   return m_opts;
}

//===========================================
// EntityPhysics::setPhysOptions
//===========================================
void EntityPhysics::setPhysOptions(const options_t& opts) {
   if (m_init)
      throw Exception("Unable to set physics options on Entity; is already initialised", __FILE__, __LINE__);

   m_opts = opts;
}

//===========================================
// EntityPhysics::addToWorld
//===========================================
void EntityPhysics::addToWorld() {
   if (m_body) {
      m_world.DestroyBody(m_body);
      m_numFixtures = 0;
   }

   constructBody();
   m_physEnts[m_entity] = this;
   m_init = true;
}

//===========================================
// EntityPhysics::removeFromWorld
//===========================================
void EntityPhysics::removeFromWorld() {
   if (m_init) {
      m_physEnts.erase(m_entity);
      if (m_body) m_world.DestroyBody(m_body);

      m_body = NULL;
      m_numFixtures = 0;

      m_init = false;
   }
}

//===========================================
// EntityPhysics::constructBody
//===========================================
void EntityPhysics::constructBody() {
   b2BodyDef bdef;

   if (m_opts.dynamic) bdef.type = b2_dynamicBody;
   bdef.fixedRotation = m_opts.fixedAngle;

   bdef.position.Set(static_cast<float>(m_entity->getPosition().x) / m_scaledPixPerMetre,
      static_cast<float>(m_entity->getPosition().y) / m_scaledPixPerMetre);

   m_body = m_world.CreateBody(&bdef);

   const CompoundPoly& compoundPoly = m_entity->getBoundingPoly();
   for (unsigned int p = 0; p < compoundPoly.getNumPolys(); p++) {
      const Poly& poly = compoundPoly.getPoly(p);

      b2Vec2* verts = new b2Vec2[poly.verts.size()];
      for (unsigned int i = 0; i < poly.verts.size(); i++)
         verts[i] = b2Vec2(static_cast<float>(poly.verts[i].x) / m_scaledPixPerMetre,
            static_cast<float>(poly.verts[i].y) / m_scaledPixPerMetre);

      b2PolygonShape b2Poly;
      b2Poly.Set(verts, poly.verts.size());

      b2FixtureDef fdef;
      fdef.shape = &b2Poly;
      fdef.density = m_opts.density;
      fdef.friction = m_opts.friction;

      m_body->CreateFixture(&fdef);
      m_numFixtures++;
   }
}

//===========================================
// EntityPhysics::makeDynamic
//===========================================
void EntityPhysics::makeDynamic() {
   if (!m_init)
      throw Exception("Instance of EntityPhysics is not initialised", __FILE__, __LINE__);

   if (m_body->GetType() == b2_dynamicBody)
      return;

   m_world.DestroyBody(m_body);
   m_numFixtures = 0;

   m_opts.dynamic = true;
   constructBody();
}

//===========================================
// EntityPhysics::makeStatic
//===========================================
void EntityPhysics::makeStatic() {
   if (!m_init)
      throw Exception("Instance of EntityPhysics is not initialised", __FILE__, __LINE__);

   if (m_body->GetType() != b2_dynamicBody)
      return;

   m_world.DestroyBody(m_body);
   m_numFixtures = 0;

   m_opts.dynamic = false;
   constructBody();
}

//===========================================
// EntityPhysics::loadSettings
//===========================================
void EntityPhysics::loadSettings(const string& file) {
   KvpParser parser;
   parser.parseFile(file);

   if (parser.getMetaData(0).compare("Box2D") != 0)
      throw Exception("Error loading settings; file not for this implementation (Box2D)", __FILE__, __LINE__);

   m_scaledPixPerMetre = atof(parser.getValue("pixelsPerMetre").data());
   m_v_iterations = atoi(parser.getValue("vIterations").data());
   m_p_iterations = atoi(parser.getValue("pIterations").data());

   Functor<void, TYPELIST_1(pEEvent_t)> fEntMovedHandler(&EntityPhysics::entityMovedHandler);
   m_eventManager.registerCallback(internString("entityMoved"), fEntMovedHandler);
}

//===========================================
// EntityPhysics::entityMovedHandler
//===========================================
void EntityPhysics::entityMovedHandler(pEEvent_t ev) {
   static long entityMovedStr = internString("entityMoved");
   if (ev->getType() != entityMovedStr)
      throw Exception("Error handling entityMoved event; unexpected event type", __FILE__, __LINE__);

   EEntityMoved* event = static_cast<EEntityMoved*>(ev.get());

   if (m_ignore.find(event) == m_ignore.end()) {
      map<Entity*, EntityPhysics*>::iterator it = m_physEnts.find(event->getEntity().get());
      if (it != m_physEnts.end()) it->second->updatePos(event);
   }
   else
      m_ignore.erase(event);
}

//===========================================
// EntityPhysics::updatePos
//===========================================
void EntityPhysics::updatePos(EEntityMoved* event) {
   if (!m_init)
      throw Exception("Instance of EntityPhysics is not initialised", __FILE__, __LINE__);

   // Update position
   double entX = event->getNewPos().x;
   double entY = event->getNewPos().y;
   double entA = event->getNewAngle();
   m_body->SetTransform(b2Vec2(entX / m_scaledPixPerMetre, entY / m_scaledPixPerMetre), entA);


   // Update poly

   CompoundPoly oldPoly = event->getOldSrcPoly();
   CompoundPoly newPoly = event->getNewSrcPoly();
   oldPoly.scaleFromZero(event->getOldScale());
   newPoly.scaleFromZero(event->getNewScale());

   if (oldPoly != newPoly) {
      float d = m_body->GetFixtureList()->GetDensity();
      float f = m_body->GetFixtureList()->GetFriction();

      for (unsigned int i = 0; i < m_numFixtures; i++)
         m_body->DestroyFixture(m_body->GetFixtureList());

      m_numFixtures = 0;

      for (unsigned int p = 0; p < newPoly.getNumPolys(); p++) {
         const Poly& poly = newPoly.getPoly(p);

         b2Vec2* verts = new b2Vec2[poly.verts.size()];
         for (unsigned int i = 0; i < poly.verts.size(); i++)
            verts[i] = b2Vec2(static_cast<float>(poly.verts[i].x) / m_scaledPixPerMetre,
               static_cast<float>(poly.verts[i].y) / m_scaledPixPerMetre);

         b2PolygonShape b2Poly;
         b2Poly.Set(verts, poly.verts.size());

         b2FixtureDef fdef;
         fdef.shape = &b2Poly;
         fdef.density = d;
         fdef.friction = f;

         m_body->CreateFixture(&fdef);
         m_numFixtures++;
      }
   }

   // Wake bodies
   for (map<Entity*, EntityPhysics*>::iterator it = m_physEnts.begin(); it != m_physEnts.end(); ++it)
      it->second->m_body->SetAwake(true);
}

//===========================================
// EntityPhysics::updateFrameRate
//===========================================
void EntityPhysics::updateFrameRate() {
   static Timer timer;
   static long i = 0; i++;

   if (i % 10 == 0) {
      m_frameRate = 10.0 / timer.getTime();
      timer.reset();
   }
}

//===========================================
// EntityPhysics::update
//
//! @brief Call in main loop.
//===========================================
void EntityPhysics::update() {
   updateFrameRate();
   if (m_physEnts.empty()) return;


   //---------Update b2Body positions---------
   m_world.Step(1.0 / m_frameRate, m_v_iterations, m_p_iterations);
   m_world.ClearForces();
   //-----------------------------------------


   //-----Match Entity to b2Body position-----
   for (map<Entity*, EntityPhysics*>::iterator it = m_physEnts.begin(); it != m_physEnts.end(); ++it) {
      if (!it->second->m_opts.dynamic) continue;

      Entity* ent = it->second->m_entity;

      Vec2i pos(it->second->m_body->GetPosition()(0) * m_scaledPixPerMetre,
         it->second->m_body->GetPosition()(1) * m_scaledPixPerMetre);
      double a = it->second->m_body->GetAngle();

      Vec2i posDiff = ent->getPosition() - pos;
      double aDiff = ent->getAngle() - a;
      if (!(abs(posDiff.x) >= 1 || abs(posDiff.y) >= 1 || fabs(aDiff) >= 0.01)) continue;

      try {
         Vec2f oldPos = ent->getPosition();
         double oldA = ent->getAngle();

         ent->setPosition_s(pos.x, pos.y);
         ent->setAngle_s(a);

         pEEntityMoved_t event(new EEntityMoved(ent->getSharedPtr(), oldPos, pos,
            ent->getSourcePoly(), ent->getSourcePoly(), ent->getScaleVector(), ent->getScaleVector(), oldA, a));
         m_ignore.insert(event.get());
         m_eventManager.queueEvent(event);
      }
      catch (bad_alloc& e) {
         Exception ex("Error updating entity physics; ", __FILE__, __LINE__);
         ex.append(e.what());
         throw ex;
      }
   }
   //-----------------------------------------
}

//===========================================
// EntityPhysics::getLinearVelocity
//===========================================
Vec2f EntityPhysics::getLinearVelocity() const {
   if (!m_init)
      throw Exception("Instance of EntityPhysics is not initialised", __FILE__, __LINE__);

   return Vec2f(m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y);
}

//===========================================
// EntityPhysics::applyLinearImpulse
//===========================================
void EntityPhysics::applyLinearImpulse(const Vec2f& impulse, const Vec2f& p) {
   if (!m_init)
      throw Exception("Instance of EntityPhysics is not initialised", __FILE__, __LINE__);

   m_body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(static_cast<float>(p.x) / m_scaledPixPerMetre,
      static_cast<float>(p.y) / m_scaledPixPerMetre));
}

//===========================================
// EntityPhysics::applyForce
//===========================================
void EntityPhysics::applyForce(const Vec2f& force, const Vec2f& p) {
   if (!m_init)
      throw Exception("Instance of EntityPhysics is not initialised", __FILE__, __LINE__);

   m_body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(static_cast<float>(p.x) / m_scaledPixPerMetre,
      static_cast<float>(p.y) / m_scaledPixPerMetre));
}


}
