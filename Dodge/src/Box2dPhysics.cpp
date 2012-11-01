/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <Box2dPhysics.hpp>
#include <EEntityMoved.hpp>
#include <Timer.hpp>
#include <KvpParser.hpp>
#include <Exception.hpp>
#include <StringId.hpp>
#include <utils/Functor.hpp>


using namespace std;


namespace Dodge {


float Box2dPhysics::m_frameRate = 60.0;
float Box2dPhysics::m_scaledPixPerMetre = 8.0;
int Box2dPhysics::m_v_iterations = 6;
int Box2dPhysics::m_p_iterations = 4;
EventManager Box2dPhysics::m_eventManager = EventManager();
set<EEvent*> Box2dPhysics::m_ignore = set<EEvent*>();
map<Entity*, Box2dPhysics*> Box2dPhysics::m_physEnts = map<Entity*, Box2dPhysics*>();
b2Vec2 Box2dPhysics::m_gravity = b2Vec2(0.0, -9.8);
b2World Box2dPhysics::m_world = b2World(m_gravity, true);


//===========================================
// Box2dPhysics::deepCopy
//===========================================
void Box2dPhysics::deepCopy(const Box2dPhysics& copy) {
/*
      bool m_init;

      b2Body* m_body;
      Entity* m_entity;
      unsigned int m_numFixtures;

      bool m_dynamic;
      bool m_fixedAngle;
      float32_t m_density;
      float32_t m_friction;
*/

   m_init = false;
   // TODO
}

//===========================================
// Box2dPhysics::clone
//===========================================
EntityPhysicsImpl* Box2dPhysics::clone() const {
   return new Box2dPhysics(*this);
}

//===========================================
// Box2dPhysics::assignData
//===========================================
void Box2dPhysics::assignData(const rapidxml::xml_node<>* data) {
   // TODO
}

#ifdef DEBUG
//===========================================
// Box2dPhysics::dbg_print
//===========================================
void Box2dPhysics::dbg_print(std::ostream& out, int tab) const {
   // TODO
}
#endif

//===========================================
// Box2dPhysics::setEntity
//===========================================
void Box2dPhysics::setEntity(Entity* entity) {
   m_entity = entity;
}

//===========================================
// Box2dPhysics::addToWorld
//===========================================
void Box2dPhysics::addToWorld() {
   if (m_body) {
      m_world.DestroyBody(m_body);
      m_numFixtures = 0;
   }

   constructBody();
   m_physEnts[m_entity] = this;
   m_init = true;
}

//===========================================
// Box2dPhysics::removeFromWorld
//===========================================
void Box2dPhysics::removeFromWorld() {
   if (m_init) {
      m_physEnts.erase(m_entity);
      if (m_body) m_world.DestroyBody(m_body);

      m_body = NULL;
      m_numFixtures = 0;

      m_init = false;
   }
}

//===========================================
// Box2dPhysics::constructBody
//===========================================
void Box2dPhysics::constructBody() {/*
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
   }*/
}

//===========================================
// Box2dPhysics::makeDynamic
//===========================================
void Box2dPhysics::makeDynamic() {
   if (!m_init)
      throw Exception("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   if (m_body->GetType() == b2_dynamicBody)
      return;

   m_world.DestroyBody(m_body);
   m_numFixtures = 0;

   m_dynamic = true;
   constructBody();
}

//===========================================
// Box2dPhysics::makeStatic
//===========================================
void Box2dPhysics::makeStatic() {
   if (!m_init)
      throw Exception("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   if (m_body->GetType() != b2_dynamicBody)
      return;

   m_world.DestroyBody(m_body);
   m_numFixtures = 0;

   m_dynamic = false;
   constructBody();
}

//===========================================
// Box2dPhysics::loadSettings
//===========================================
void Box2dPhysics::loadSettings(const string& file) {
   KvpParser parser;
   parser.parseFile(file);

   if (parser.getMetaData(0).compare("Box2D") != 0)
      throw Exception("Error loading settings; file not for this implementation (Box2D)", __FILE__, __LINE__);

   m_scaledPixPerMetre = atof(parser.getValue("pixelsPerMetre").data());
   m_v_iterations = atoi(parser.getValue("vIterations").data());
   m_p_iterations = atoi(parser.getValue("pIterations").data());

   Functor<void, TYPELIST_1(EEvent*)> fEntMovedHandler(&Box2dPhysics::entityMovedHandler);
   m_eventManager.registerCallback(internString("entityMoved"), fEntMovedHandler);
}

//===========================================
// Box2dPhysics::entityMovedHandler
//===========================================
void Box2dPhysics::entityMovedHandler(EEvent* ev) {
   static long entityMovedStr = internString("entityMoved");
   if (ev->getType() != entityMovedStr)
      throw Exception("Error handling entityMoved event; unexpected event type", __FILE__, __LINE__);

   EEntityMoved* event = static_cast<EEntityMoved*>(ev);

   if (m_ignore.find(event) == m_ignore.end()) {
      map<Entity*, Box2dPhysics*>::iterator it = m_physEnts.find(event->entity.get());
      if (it != m_physEnts.end()) it->second->updatePos(event);
   }
   else
      m_ignore.erase(event);
}

//===========================================
// Box2dPhysics::updatePos
//===========================================
void Box2dPhysics::updatePos(EEntityMoved* event) {/*
   if (!m_init)
      throw Exception("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

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
   for (map<Entity*, Box2dPhysics*>::iterator it = m_physEnts.begin(); it != m_physEnts.end(); ++it)
      it->second->m_body->SetAwake(true);*/
}

//===========================================
// Box2dPhysics::updateFrameRate
//===========================================
void Box2dPhysics::updateFrameRate() {
   static Timer timer;
   static long i = 0; i++;

   if (i % 10 == 0) {
      m_frameRate = 10.0 / timer.getTime();
      timer.reset();
   }
}

//===========================================
// Box2dPhysics::update
//
//! @brief Call in main loop.
//===========================================
void Box2dPhysics::update() {/*
   updateFrameRate();
   if (m_physEnts.empty()) return;


   //---------Update b2Body positions---------
   m_world.Step(1.0 / m_frameRate, m_v_iterations, m_p_iterations);
   m_world.ClearForces();
   //-----------------------------------------


   //-----Match Entity to b2Body position-----
   for (map<Entity*, Box2dPhysics*>::iterator it = m_physEnts.begin(); it != m_physEnts.end(); ++it) {
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
*/}

//===========================================
// Box2dPhysics::getLinearVelocity
//===========================================
Vec2f Box2dPhysics::getLinearVelocity() const {
   if (!m_init)
      throw Exception("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   return Vec2f(m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y);
}

//===========================================
// Box2dPhysics::applyLinearImpulse
//===========================================
void Box2dPhysics::applyLinearImpulse(const Vec2f& impulse, const Vec2f& p) {
   if (!m_init)
      throw Exception("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   m_body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(static_cast<float>(p.x) / m_scaledPixPerMetre,
      static_cast<float>(p.y) / m_scaledPixPerMetre));
}

//===========================================
// Box2dPhysics::applyLinearImpulse
//===========================================
void Box2dPhysics::applyLinearImpulse(const Vec2f& impulse) {
   if (!m_init) return;
   applyLinearImpulse(impulse, Vec2f(m_body->GetWorldCenter().x * m_scaledPixPerMetre, m_body->GetWorldCenter().y * m_scaledPixPerMetre));
}

//===========================================
// Box2dPhysics::applyForce
//===========================================
void Box2dPhysics::applyForce(const Vec2f& force, const Vec2f& p) {
   if (!m_init)
      throw Exception("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   m_body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(static_cast<float>(p.x) / m_scaledPixPerMetre,
      static_cast<float>(p.y) / m_scaledPixPerMetre));
}

//===========================================
// Box2dPhysics::applyForce
//===========================================
void Box2dPhysics::applyForce(const Vec2f& force) {
   if (!m_init) return;
   applyForce(force, Vec2f(m_body->GetWorldCenter().x * m_scaledPixPerMetre, m_body->GetWorldCenter().y * m_scaledPixPerMetre));
}


}
